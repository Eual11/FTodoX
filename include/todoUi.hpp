#ifndef _TODO_UI_HPP
#define _TODO_UI_HPP
#include "./todoCore.hpp"
#include "transforms.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <yaml-cpp/emitter.h>
#include <yaml-cpp/node/parse.h>
// TODO: SEVERAL BUG FIXES, the save and load are path dependant
//  TODO: an ID for each task
//   BUG: the status line gets squashed
//    TODO: defeault input option
//     the UI is one big component that holds two pain components namely
//    TODO: delete opration is vage
//     workspacePanel: is a vertical container with a given selector
//     tasksWindow- is a veretical container that holds the tasks
//
//     implemented tasks: inseertion, renaming /editing of both tasks and
//     workplaces
class todoUi : public ftxui::ComponentBase {
private:
  int workspaceSelected = 0;
  int maintaskSelected = 0;

  std::vector<todoCore::Workspace> workspaces;
  std::string wpName = "";
  std::string taskDesc = "";
  bool isInputState = false;
  ftxui::Component newInput;
  ftxui::MenuEntryOption defaultTasksOption;
  ftxui::MenuEntryOption defaultWorkspaceOption;
  ftxui::Component workspacePanel =
      ftxui::Container::Vertical({}, &workspaceSelected);

  ftxui::Component tasksWindow =
      ftxui::Container::Vertical({}, &maintaskSelected);

  bool OnEvent(ftxui::Event event) override {
    if (isInputState && !newInput->Active()) {
      updataWorkplaceView();
      updateTasksView(workspaceSelected, maintaskSelected);
      isInputState = false;
      return true;
    }
    if (!isInputState) {
      if (event == ftxui::Event::Return) {
        if (workspacePanel->Focused()) {
          updateAllView(workspaceSelected, maintaskSelected);
          if (tasksWindow->ChildCount() > 0)
            tasksWindow->ChildAt(0)->TakeFocus();
        }
        // WARNING: MAYEB NOT?
        return true;
      }
      if (event == ftxui::Event::Character('a')) {
        // insertion operatipn
        //
        //
        if (workspacePanel->Focused() || workspaces.size() == 0) {
          // insertion to workplaces
          createWorkspace(workspaceSelected + 1);
        } else if (tasksWindow->Focused()) {
          // insertion on tasks
          //
          createTask(workspaceSelected, maintaskSelected + 1);
        }
        saveData();
        return true;
      }
      if (event == ftxui::Event::Character('c')) {
        // complete a task
        // horrible
        if (tasksWindow->Focused() && workspaces.size() > 0 &&
            workspaces[workspaceSelected].tasks.size() > 0) {
          auto &wp = workspaces[workspaceSelected].tasks[maintaskSelected];
          wp.setCompleted();
        }
        updateAllView(workspaceSelected, maintaskSelected);
        saveData();
        return true;
      }

      if (event == ftxui::Event::Character('d')) {
        // delete operation
        //
        if (workspacePanel->Focused()) {
          if (workspaces.size() > 0) {
            workspaces.erase(workspaces.begin() + workspaceSelected);
            updataWorkplaceView(workspaceSelected - 1);
            std::cerr << "Wp after deletion: " << workspaceSelected << "\n";
            updateTasksView(workspaceSelected, maintaskSelected);
            // focusing to task window if there are any tasks in the workspace
            // after deletion
            if (workspaces.size() > 0)
              tasksWindow->TakeFocus();
          }
        } else if (tasksWindow->Focused()) {
          if (workspaces.size() > 0) {
            auto &wpTasks = workspaces[workspaceSelected].tasks;
            if (wpTasks.size() > 0) {
              std::cerr << "wp: " << workspaceSelected
                        << "ms: " << maintaskSelected << "\n";
              wpTasks.erase(wpTasks.begin() + maintaskSelected);
              updateTasksView(workspaceSelected, maintaskSelected);
            }
          }
        }

        saveData();
        return true;
      }
      if (event == ftxui::Event::Character('r')) {
        // rename opration
        //
        if (workspacePanel->Focused()) {
          // rename
          //

          renameWorkplace(workspaceSelected);
        } else if (tasksWindow->Focused()) {
          editTask(workspaceSelected, maintaskSelected);
        }

        saveData();
        return true;
      }
    }

    return ftxui::ComponentBase::OnEvent(event);
  }

  void insertWorkspace(size_t index, todoCore::Workspace &workspace) {
    workspaces.insert(workspaces.begin() + index, workspace);
    updataWorkplaceView();
    isInputState = false;
  }
  void createWorkspace(size_t index) {
    ftxui::InputOption option;
    option.multiline = false;
    option.placeholder = "Workspace " + std::to_string(workspaces.size() + 1);

    option.on_enter = [&] {
      // NOTE: this could be a function itself
      if (wpName.length() == 0)
        wpName = "Workspace " + std::to_string(workspacePanel->ChildCount());
      ;
      todoCore::Workspace wp(workspaceSelected, wpName);
      insertWorkspace(workspaceSelected, wp);
      updateAllView(workspaceSelected, maintaskSelected);
      updataWorkplaceView(workspaceSelected);
      tasksWindow->TakeFocus();
      saveData();
    };

    newInput = Input(&wpName, option);
    /* auto newPanel = ftxui::Container::Vertical({}, &workspaceSelected); */
    // inserting the input element in to the input panel
    //
    //
    // if the work space is empty
    //
    size_t size = workspaces.size();
    workspacePanel->DetachAllChildren();
    for (size_t i = 0; i <= size; i++) {
      if (index == i || workspaces.size() == 0)
        workspacePanel->Add(newInput);
      if (i < workspaces.size()) {
        workspacePanel->Add(
            ftxui::MenuEntry(workspaces[i].name, defaultWorkspaceOption));
      }
    }

    isInputState = true;
    wpName = "";
    newInput->TakeFocus();
  }
  void editTask(size_t workspaceIndex, size_t taskIndex) {
    if (workspaceIndex >= workspaces.size())
      return;

    auto &wpTasks = workspaces[workspaceIndex].tasks;
    if (taskIndex >= wpTasks.size())
      return;
    ftxui::InputOption option;
    option.multiline = false;
    taskDesc = wpTasks[taskIndex].desc;
    option.placeholder = taskDesc;
    option.on_enter = [&] {
      if (taskDesc != "")
        wpTasks[maintaskSelected].desc = taskDesc;

      updateTasksView(workspaceSelected, maintaskSelected);
      isInputState = false;
      saveData();
    };

    newInput = Input(&taskDesc, option);

    tasksWindow->DetachAllChildren();

    for (size_t i = 0; i <= wpTasks.size(); i++) {
      if (taskIndex == i) {
        tasksWindow->Add(newInput);
        continue;
      }
      if (i < wpTasks.size()) {
        tasksWindow->Add(ftxui::MenuEntry(wpTasks[i].desc));
      }
    }

    isInputState = true;
    taskDesc = "";
    newInput->TakeFocus();
  }
  ftxui::Element Render() override {

    auto mainView =
        ftxui::hbox({workspacePanel->Render() |
                         ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 30) |
                         ftxui::frame | ftxui::border,

                     tasksWindow->Render() | ftxui::border | ftxui::flex_grow});

    auto statusLine =
        ftxui::hbox({ftxui::text("Status") | ftxui::flex_grow | ftxui::border});

    return ftxui::window(ftxui::text("FTXodo") | ftxui::center,
                         ftxui::vbox({mainView | ftxui::flex_grow,
                                      statusLine | ftxui::xflex_grow}));
    //|
    //     ftxui::bgcolor(ftxui::Color(hexToRGB("#21283b")));
  }

  ftxui::Color hexToRGB(std::string hex) {

    int i = 0;
    uint32_t rgb = 0x00;
    for (std::string::reverse_iterator iter = hex.rbegin(); iter != hex.rend();
         iter++, i++) {
      unsigned int val = 0;
      if (std::isalpha(*iter) && *iter >= 'a' && *iter <= 'f') {

        val = 10 + *iter - 'a';

      } else if (*iter >= '0' && *iter <= '9') {
        val = *iter - '0';
      } else
        val = 0;
      rgb += val * std::pow(16, i);
    }
    return ftxui::Color(rgb >> 16, (rgb & 0xff00) >> 8, rgb & 0xff);
    /* return rgb >> 8 & 0x00ff00; */
  }

  //@breif: loads tasks and subsquent workplaces
  //
  //

  void loadData() {
    // do something
    //
    //
    // TODO: better name, better implementation
    YAML::Node nodeData = YAML::LoadFile("../test.yaml");
    if (nodeData.size() == 0 || !nodeData.IsSequence()) {
      std::cerr << "Couldn't Open/Load Saved Todos :(";
      return;
    }

    for (auto wp : nodeData) {
      workspaces.push_back(wp.as<todoCore::Workspace>());
    }
  }
  void saveData() {
    // TODO: better name, implementation
    YAML::Emitter emitter;
    emitter << YAML::BeginSeq;

    for (auto wp : workspaces) {
      emitter << wp;
    }
    emitter << YAML::EndSeq;

    std::ofstream out("../test.yaml");
    if (out.fail()) {
      std::cerr << "Couldn't save\n";
      return;
    }
    out << emitter.c_str();
    out.close();
  }
  void updataWorkplaceView(int index = -1) {
    index = index < 0 ? workspaceSelected : index;
    // cannot be loaded unless there are loaded tasks
    workspacePanel->DetachAllChildren();
    int wp_selected = 0;
    for (auto &wp : workspaces) {
      // testing the style
      //
      if (wp_selected == index)
        defaultWorkspaceOption.transform = selectedWorkspaceStyle;
      else
        defaultWorkspaceOption.transform = defaultWorkspaceStyle;
      workspacePanel->Add(ftxui::MenuEntry(wp.name, defaultWorkspaceOption));
      wp_selected++;
    }
    defaultWorkspaceOption.transform = defaultWorkspaceStyle;
    // adding a dummy menu entry
    if (workspacePanel->ChildCount() == 0) {
      defaultWorkspaceOption.transform = selectedWorkspaceStyle;
      workspacePanel->Add(
          ftxui::MenuEntry("Add New Workspace", defaultWorkspaceOption));
      workspacePanel->SetActiveChild(workspacePanel->ChildAt(0));
    } else if (index < (int)workspacePanel->ChildCount()) {
      workspacePanel->SetActiveChild(workspacePanel->ChildAt(index));
    }
  }
  void updateTasksView(size_t workspaceIndex, size_t taskIndex) {
    // uodate the tasks based on the selected workplace
    tasksWindow->DetachAllChildren();
    if (workspaceIndex < workspaces.size())
      for (auto &task : workspaces[workspaceIndex].tasks) {
        switch (task.status) {
        case todoCore::TaskStatus::STARTED: {
          defaultTasksOption.transform = defaultTaskStyle;
          break;
        }
        case todoCore::TaskStatus::COMPLETED: {
          defaultTasksOption.transform = defaultCompletedTaskStyle;
          break;
        }
        case todoCore::TaskStatus::OVERDUE: {
          break;
        }
        }
        tasksWindow->Add(ftxui::MenuEntry(task.desc, defaultTasksOption));
      }
    defaultTasksOption.transform = {};
    if (tasksWindow->ChildCount() == 0)
      tasksWindow->Add(ftxui::MenuEntry("Add New Task", defaultTasksOption));
    if (taskIndex > 0)
      tasksWindow->SetActiveChild(tasksWindow->ChildAt(taskIndex - 1));
  }
  void updateAllView(int workspaceIndex, int taskIndex) {
    updataWorkplaceView(workspaceIndex);
    updateTasksView(workspaceIndex, taskIndex);
  }
  void renameWorkplace(size_t index) {
    if (index >= workspaces.size())
      return;
    ftxui::InputOption option;
    option.multiline = false;
    wpName = workspaces[workspaceSelected].getName();
    // option.placeholder = workspaces[workspaceSelected].getName();
    option.placeholder = wpName;
    option.on_enter = [&] {
      if (wpName.length() == 0)
        wpName = workspaces[workspaceSelected].getName();
      workspaces[workspaceSelected].setName(wpName);
      updataWorkplaceView();
      isInputState = false;
      saveData();
    };
    newInput = Input(&wpName, option);
    workspacePanel->DetachAllChildren();
    for (size_t i = 0; i < workspaces.size(); i++) {
      if (index == i) {
        workspacePanel->Add(newInput);
        continue;
      }
      workspacePanel->Add(
          ftxui::MenuEntry(workspaces[i].name, defaultWorkspaceOption));
    }

    isInputState = true;
    wpName = "";
    newInput->TakeFocus();
  }
  void createTask(int workspaceIndex, int taskSelected) {
    if ((size_t)workspaceIndex >= workspaces.size())
      return;

    ftxui::InputOption option;
    option.multiline = false;
    option.placeholder =
        "Task " + std::to_string(workspaces[workspaceIndex].tasks.size() + 1);
    option.on_enter = [&] {
      std::vector<todoCore::TodoTask> &workspaceTasks =
          workspaces[workspaceSelected].tasks;
      if (taskDesc.length() != 0)
        workspaceTasks.insert(workspaceTasks.begin() + maintaskSelected,
                              todoCore::TodoTask(maintaskSelected, taskDesc));

      updateTasksView(workspaceSelected, maintaskSelected + 1);
      isInputState = false;
      saveData();
    };

    newInput = ftxui::Input(&taskDesc, option);

    tasksWindow->DetachAllChildren();

    std::vector<todoCore::TodoTask> workspaceTasks =
        workspaces[workspaceIndex].tasks; // WARNING: maybe not

    size_t size = workspaceTasks.size();
    for (size_t i = 0; i <= size; i++) {
      if (taskSelected == i || workspaceTasks.size() == 0) {
        // insert input element here
        tasksWindow->Add(newInput);
      }
      if (i < workspaceTasks.size()) {
        switch (workspaceTasks[i].status) {
        case todoCore::TaskStatus::STARTED: {
          defaultTasksOption.transform = defaultTaskStyle;
          break;
        }
        case todoCore::TaskStatus::COMPLETED: {
          defaultTasksOption.transform = defaultCompletedTaskStyle;
          break;
        }
        case todoCore::TaskStatus::OVERDUE: {
          break;
        }
        }
        tasksWindow->Add(
            ftxui::MenuEntry(workspaceTasks[i].desc, defaultTasksOption));
      }
      isInputState = true;
      taskDesc = ""; //"Task " + std::to_string(size + 1);
      newInput->TakeFocus();
      // std::vector<
    }
  }

public:
  todoUi() {
    std::cout << "Shit Happend\n";
    // test for the container
    auto comp = ftxui::Container::Horizontal({workspacePanel, tasksWindow});
    /* loadTasks(); */

    defaultTasksOption.transform = defaultTaskStyle;

    defaultWorkspaceOption.transform = defaultWorkspaceStyle;
    loadData();
    updataWorkplaceView();

    updateTasksView(workspaceSelected, maintaskSelected);

    Add(comp);
  }
};

#endif
