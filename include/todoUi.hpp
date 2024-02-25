#ifndef _TODO_UI_HPP
#define _TODO_UI_HPP
#include "./todoCore.hpp"
#include "Transformer.hpp"
#include "Utils.hpp"
#include "transforms.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <yaml-cpp/emitter.h>
#include <yaml-cpp/node/parse.h>

// TODO: Sorting todos
// TODO: UTF-8 support through wstrings
//  TODO: SEVERAL BUG FIXES, the save and load are path dependant
//   TODO: an ID for each task
//    BUG: the status line gets squashed
//     TODO: defeault input option
//      the UI is one big component that holds two pain components namely
//     TODO: delete opration is vage
//      workspacePanel: is a vertical container with a given selector
//      tasksWindow- is a veretical container that holds the tasks

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
  Transformer todoTransformer;

  ftxui::MenuEntryOption defaultTasksOption;
  ftxui::MenuEntryOption defaultWorkspaceOption;
  ftxui::Component workspacePanel =
      ftxui::Container::Vertical({}, &workspaceSelected);

  ftxui::Component tasksWindow =
      ftxui::Container::Vertical({}, &maintaskSelected);

  bool OnEvent(ftxui::Event event) override {
    if (isInputState && !newInput->Active()) {
      updateWorkplaceView();
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
          wp.toggleCompleted();
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
            updateWorkplaceView(workspaceSelected - 1);
            std::cerr << "Wp after deletion: " << workspaceSelected << "\n";
            updateTasksView(workspaceSelected, maintaskSelected - 1);
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
              updateTasksView(workspaceSelected, maintaskSelected - 1);
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
    updateWorkplaceView();
    isInputState = false;
  }
  void createWorkspace(size_t index) {
    ftxui::InputOption option;
    option.multiline = false;
    option.placeholder = "Workspace " + std::to_string(workspaces.size() + 1);
    todoCore::Workspace dummy(1, "dummy");
    dummy.tasks = {};
    option.transform = todoTransformer.StyleWorkspaceInput(dummy);
    option.on_enter = [&] {
      // NOTE: this could be a function itself
      if (wpName.length() == 0)
        wpName = "Workspace " + std::to_string(workspacePanel->ChildCount());
      ;
      todoCore::Workspace wp(workspaceSelected, wpName);
      insertWorkspace(workspaceSelected, wp);
      updateAllView(workspaceSelected, maintaskSelected);
      updateWorkplaceView(workspaceSelected);
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
    option.content = taskDesc;
    option.transform = todoTransformer.StyleTaskInput(wpTasks[taskIndex]);
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
        addToTaskWindow(wpTasks[i]);
        /* tasksWindow->Add(ftxui::MenuEntry(wpTasks[i].desc)); */
      }
    }

    isInputState = true;
    taskDesc = "";
    newInput->TakeFocus();
  }
  ftxui::Element Render() override {

    using namespace ftxui;
    // TODO: make this neater any how
    // basically all it does is styling the header for workspace and tasks
    // window based on their focus state
    auto workspaceHeaderColor = color(hexToRGB("#080716"));
    auto workspaceHeaderBGcolor = bgcolor(hexToRGB("#809be5"));
    if (!workspacePanel->Focused()) {
      workspaceHeaderColor = color(hexToRGB("#e7ebfa"));
      workspaceHeaderBGcolor = bgcolor(hexToRGB("#363e5b"));
    }
    auto tasksHeaderColor = color(hexToRGB("#080716"));
    auto tasksHeaderBGcolor = bgcolor(hexToRGB("#809be5"));
    if (!tasksWindow->Focused()) {
      tasksHeaderColor = color(hexToRGB("#e7ebfa"));
      tasksHeaderBGcolor = bgcolor(hexToRGB("#363e5b"));
    }
    // the text element with a filler is the header the empty text("") acts as a
    // seprarator (maybe change it to an empty element)
    auto workspaceview =
        vbox({hbox({text("Workspaces"), filler()}) | workspaceHeaderBGcolor |
                  workspaceHeaderColor | hcenter,
              text(""), workspacePanel->Render()});

    auto taskView = vbox({hbox({text("Tasks "), filler()}) | tasksHeaderColor |
                              tasksHeaderBGcolor | center,
                          text(""), tasksWindow->Render()});

    auto mainView = ftxui::hbox(
        {workspaceview | hcenter | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 30) |
             ftxui::frame | todoBorderStyle,

         taskView | vscroll_indicator | flex | yframe | todoBorderStyle});
    // TODO: styling for statys line
    auto statusLine = ftxui::hbox(
        {ftxui::text("Normal ") | statusLineColor | statusLineBGcolor,
         ftxui::filler()});

    return ftxui::vbox({mainView | ftxui::flex,
                        statusLine |
                            ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1) |
                            ftxui::xflex_grow}) |
           ftxui::bgcolor(ftxui::Color(hexToRGB(todoTransformer.todobgColor)));
    //|
    //     ftxui::bgcolor(ftxui::Color(hexToRGB("#21283b")));
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
  void updateWorkplaceView(int index = -1) {
    index = index < 0 ? workspaceSelected : index;
    // cannot be loaded unless there are loaded tasks
    workspacePanel->DetachAllChildren();
    int wp_selected = 0;
    for (auto &wp : workspaces) {
      // testing the style
      //
      //
      defaultWorkspaceOption.transform =
          todoTransformer.StyleWorkspace(wp, index == wp_selected);
      workspacePanel->Add(ftxui::MenuEntry(wp.name, defaultWorkspaceOption));
      wp_selected++;
    }
    todoCore::Workspace dummy(1, "test"); // dummy empty workspace
    defaultWorkspaceOption.transform =
        todoTransformer.StyleWorkspace(dummy, false);
    // adding a dummy menu entry
    if (workspacePanel->ChildCount() == 0) {
      defaultWorkspaceOption.transform = todoTransformer.StyleWorkspace(
          dummy, true); // syling the dummy workspace
      workspacePanel->Add(
          ftxui::MenuEntry("Add New Workspace", defaultWorkspaceOption));
      workspacePanel->SetActiveChild(workspacePanel->ChildAt(0));
    } else if (index < (int)workspacePanel->ChildCount()) {
      workspacePanel->SetActiveChild(workspacePanel->ChildAt(index));
    }
  }
  void updateTasksView(int workspaceIndex, int taskIndex) {
    // uodate the tasks based on the selected workplace
    std::cerr << "Task index " << taskIndex << "\n";
    tasksWindow->DetachAllChildren();
    if (workspaceIndex < (int)workspaces.size()) {
      sortTasks(workspaces[workspaceIndex].tasks,
                todoCore::TodoTask::defaultSort);

      for (auto &task : workspaces[workspaceIndex].tasks) {
        addToTaskWindow(task);
      }
    }
    if (tasksWindow->ChildCount() == 0)
      tasksWindow->Add(ftxui::MenuEntry("Add New Task", defaultTasksOption));
    if (taskIndex >= 0)
      tasksWindow->SetActiveChild(
          tasksWindow->ChildAt(taskIndex)); // WARNING: why -1?
    else
      tasksWindow->SetActiveChild(tasksWindow->ChildAt(0));
  }
  void updateAllView(int workspaceIndex, int taskIndex) {
    updateWorkplaceView(workspaceIndex);
    updateTasksView(workspaceIndex, taskIndex);
  }
  void renameWorkplace(size_t index) {
    if (index >= workspaces.size())
      return;
    ftxui::InputOption option;
    option.multiline = false;
    option.transform =
        todoTransformer.StyleWorkspaceInput(workspaces[workspaceSelected]);
    wpName = workspaces[workspaceSelected].getName();
    // option.placeholder = workspaces[workspaceSelected].getName();
    option.placeholder = wpName;
    option.content = wpName;
    option.on_enter = [&] {
      if (wpName.length() == 0)
        wpName = workspaces[workspaceSelected].getName();
      workspaces[workspaceSelected].setName(wpName);
      updateWorkplaceView();
      isInputState = false;
      saveData();
    };
    newInput = Input(&wpName, option);
    todoCore::Workspace dummy(1, "test");
    workspacePanel->DetachAllChildren();
    for (size_t i = 0; i < workspaces.size(); i++) {
      if (index == i) {
        workspacePanel->Add(newInput);
        continue;
      }
      defaultWorkspaceOption.transform =
          todoTransformer.StyleWorkspace(workspaces[i], false);
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
    todoCore::TodoTask dummy(1, "Task");
    option.transform = todoTransformer.StyleTaskInput(dummy);
    option.placeholder =
        "Task " + std::to_string(workspaces[workspaceIndex].tasks.size() + 1);
    option.on_enter = [&] {
      std::vector<todoCore::TodoTask> &workspaceTasks =
          workspaces[workspaceSelected].tasks;
      if (taskDesc.length() != 0)
        workspaceTasks.insert(workspaceTasks.begin() + maintaskSelected,
                              todoCore::TodoTask(maintaskSelected, taskDesc));

      updateTasksView(workspaceSelected, maintaskSelected);
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
        addToTaskWindow(workspaceTasks[i]);
      }
    }
    isInputState = true;
    taskDesc = ""; //"Task " + std::to_string(size + 1);
    newInput->TakeFocus();
  }
  // adds a task to the taskWIndow
  void addToTaskWindow(todoCore::TodoTask &task) {
    ftxui::MenuEntryOption entry_option;
    entry_option.transform = todoTransformer.StyleTask(task);
    tasksWindow->Add(ftxui::MenuEntry(task.desc, entry_option));
  }
  void addToWorkspacePanel(todoCore::Workspace &wp) {
    ftxui::MenuEntryOption entry_option;
    entry_option.transform = todoTransformer.StyleWorkspace(wp, true); // BUG:
    workspacePanel->Add(ftxui::MenuEntry(wp.name, entry_option));
  }

  void sortTasks(std::vector<todoCore::TodoTask> &tasks,
                 std::function<bool(const todoCore::TodoTask &,
                                    const todoCore::TodoTask &)>
                     func) {
    std::sort(tasks.begin(), tasks.end(), func);
  }

public:
  todoUi() {
    std::cout << "Shit Happend\n";
    // test for the container
    auto comp = ftxui::Container::Horizontal({workspacePanel, tasksWindow});
    /* loadTasks(); */
    /* defaultWorkspaceOption.transform = defaultWorkspaceStyle; */
    loadData();
    updateAllView(workspaceSelected, maintaskSelected);
    Add(comp);
  }
};

#endif
