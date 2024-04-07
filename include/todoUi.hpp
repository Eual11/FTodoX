#ifndef _TODO_UI_HPP
#define _TODO_UI_HPP
#include "./todoCore.hpp"
#include "Transformer.hpp"
#include "Utils.hpp"
#include "transforms.hpp"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/task.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <functional>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <vector>
#include <yaml-cpp/emitter.h>
#include <yaml-cpp/node/parse.h>

//  TODO: Sorting todos

class todoUi : public ftxui::ComponentBase {
private:
  int workspaceSelected = 0;
  int maintaskSelected = 0;

  std::vector<todoCore::Workspace> workspaces;
  std::vector<todoCore::TodoTask> TasksList;
  std::string wpName = "";
  std::string taskDesc = "";
  std::string date = "";
  std::string searchQuery = "";
  std::string StatusLineMode = "NORMAL";
  bool isInputState = false;
  ftxui::Component newInput;
  Transformer todoTransformer;
  std::function<void()> quit;
  bool showDashBoard = true;
  std::vector<std::string> sortOptions = {"description", "date", "urgency",
                                          "status"};
  std::function<bool(const todoCore::TodoTask &, const todoCore::TodoTask &)>
      sortFunc;
  ftxui::MenuEntryOption defaultTasksOption;
  ftxui::MenuEntryOption defaultWorkspaceOption;
  ftxui::Component workspacePanel =
      ftxui::Container::Vertical({}, &workspaceSelected);

  ftxui::Component tasksWindow =
      ftxui::Container::Vertical({}, &maintaskSelected);

  ftxui::Component statusLine = ftxui::Container::Horizontal({});
  bool OnEvent(ftxui::Event event) override {
    if (isInputState && !newInput->Active()) {
      // reseting the UI if the input element is no longer active
      updateWorkplaceView();
      updateTasksView(workspaceSelected, maintaskSelected);
      updateStatusLineView(maintaskSelected);
      isInputState = false;
      return true;
    }
    if (!isInputState) {
      if (event == ftxui::Event::Return) {
        showDashBoard = false;
        if (workspacePanel->Focused()) {
          updateAllView(workspaceSelected, maintaskSelected);
          if (tasksWindow->ChildCount() > 0)
            tasksWindow->ChildAt(0)->TakeFocus();
          StatusLineMode = "NORMAL";
          std::cerr << "I CAN'T TAKE THIS PLACE\n";
          StatusLineMode = "NORMAL";
          return true;
        }
        // WARNING: MAYEB NOT?
      }

      if (event == ftxui::Event::Character('a')) {
        // insertion operatipn
        //
        //
        if (workspacePanel->Focused() || workspaces.size() == 0) {
          // insertion to workplaces
          createWorkspace(workspaceSelected + 1);
        } else if (tasksWindow->Focused() && !showDashBoard) {
          // insertion on tasks
          //
          //

          auto selectedTask =
              std::find(workspaces[workspaceSelected].tasks.begin(),
                        workspaces[workspaceSelected].tasks.end(),
                        TasksList[maintaskSelected]);
          int taskIndex = std::distance(
              workspaces[workspaceSelected].tasks.begin(), selectedTask);
          createTask(workspaceSelected, taskIndex + 1);
        }
        StatusLineMode = "INSERT";

        saveData();
        return true;
      }
      if (event == ftxui::Event::Character('c')) {
        // complete a task
        // horrible
        if (tasksWindow->Focused() && !showDashBoard && workspaces.size() > 0 &&
            workspaces[workspaceSelected].tasks.size() > 0) {
          auto &wp = workspaces[workspaceSelected].tasks[maintaskSelected];
          /* wp.toggleCompleted(); */
          auto task = std::find(workspaces[workspaceSelected].tasks.begin(),
                                workspaces[workspaceSelected].tasks.end(),
                                TasksList[maintaskSelected]);
          if (task != workspaces[workspaceSelected].tasks.end())
            task->toggleCompleted();
        }
        updateAllView(workspaceSelected, maintaskSelected);
        saveData();
        return true;
      }

      if (event == ftxui::Event::Character('d')) {

        if (tasksWindow->Focused() && !showDashBoard &&
            workspaces[workspaceSelected].tasks.size() > 0) {
          ftxui::InputOption option;
          option.multiline = false;
          option.placeholder = "Date and Time ";
          todoCore::Workspace dummy;
          option.transform = todoTransformer.StyleStatusLineInput("date");

          option.on_enter = [&] {
            isInputState = false;
            /* workspaces[workspaceSelected].tasks[maintaskSelected].parseDueDate(
             */
            /*     date); */
            auto task = std::find(workspaces[workspaceSelected].tasks.begin(),
                                  workspaces[workspaceSelected].tasks.end(),
                                  TasksList[maintaskSelected]);
            if (task != workspaces[workspaceSelected].tasks.end())
              task->parseDueDate(date);

            std::cerr << " "
                      << workspaces[workspaceSelected]
                             .tasks[maintaskSelected]
                             .getDeadline()
                      << std::endl;
            date = "";
            saveData();
            updateAllView(workspaceSelected, maintaskSelected);
          };

          newInput = ftxui::Input(&date, option);
          statusLine->DetachAllChildren();
          statusLine->Add(newInput);
          isInputState = true;
          newInput->TakeFocus();
        }

        return true;
      }
      if (event == ftxui::Event::Character('/')) {
        if (tasksWindow->Focused() && !showDashBoard) {
          ftxui::InputOption option;
          option.multiline = false;
          option.placeholder = "Find Tasks";

          option.transform = todoTransformer.StyleStatusLineInput("search");

          option.on_enter = [this] {
            updateAllView(workspaceSelected, maintaskSelected);
            searchQuery = "";
            isInputState = false;
          };

          newInput = ftxui::Input(&searchQuery, option);
          statusLine->DetachAllChildren();
          statusLine->Add(newInput);
          isInputState = true;
          newInput->TakeFocus();
        }
        return true;
      }
      if (event == ftxui::Event::Character('x')) {
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
        } else if (tasksWindow->Focused() && !showDashBoard) {
          if (workspaces.size() > 0) {
            auto &wpTasks = workspaces[workspaceSelected].tasks;
            auto task = std::find(wpTasks.begin(), wpTasks.end(),
                                  TasksList[maintaskSelected]);
            if (wpTasks.size() > 0) {
              std::cerr << "wp: " << workspaceSelected
                        << "ms: " << maintaskSelected << "\n";
              if (task != wpTasks.end())
                wpTasks.erase(task);
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
          StatusLineMode = "INSERT";
          renameWorkplace(workspaceSelected);
        } else if (tasksWindow->Focused() && !showDashBoard) {
          StatusLineMode = "INSERT";
          auto &wpTasks = workspaces[workspaceSelected].tasks;
          auto task = std::find(wpTasks.begin(), wpTasks.end(),
                                TasksList[maintaskSelected]);

          editTask(workspaceSelected,
                   (int)std::distance(wpTasks.begin(), task));
        }

        saveData();
        return true;
      }
      if (event == ftxui::Event::Character('+')) {
        if (tasksWindow->Focused() && !showDashBoard && workspaces.size() > 0 &&
            maintaskSelected <
                (int)workspaces[workspaceSelected].tasks.size()) {
          auto &wpTasks = workspaces[workspaceSelected].tasks;
          auto task = std::find(workspaces[workspaceSelected].tasks.begin(),
                                workspaces[workspaceSelected].tasks.end(),
                                TasksList[maintaskSelected]);
          if (task != wpTasks.end()) {
            task->increaseUrgency();
          }
          updateAllView(workspaceSelected, maintaskSelected);
          saveData();
        }
      }
      if (event == ftxui::Event::Character('-')) {
        if (tasksWindow->Focused() && !showDashBoard && workspaces.size() > 0 &&
            maintaskSelected <
                (int)workspaces[workspaceSelected].tasks.size()) {
          auto &wpTasks = workspaces[workspaceSelected].tasks;
          auto task = std::find(workspaces[workspaceSelected].tasks.begin(),
                                workspaces[workspaceSelected].tasks.end(),
                                TasksList[maintaskSelected]);
          if (task != wpTasks.end()) {
            task->decreaseUrgency();
          }
          updateAllView(workspaceSelected, maintaskSelected);
          saveData();
        }
      }

      if (event == ftxui::Event::Escape) {
        showDashBoard = true;
        displayDashBoard();
        workspacePanel->TakeFocus();
      }
      if (event == ftxui::Event::Character('s')) {
        // change the sorting order function to comething else
        //
        //
        if (tasksWindow->Focused()) {
          // sorting tasks

          tasksWindow->DetachAllChildren();
          maintaskSelected = 0; // reseting task selected
          ftxui::MenuOption Menuoption;

          Menuoption.on_enter = [this] {
            switch (maintaskSelected) {
            case 0: {
              sortFunc = todoCore::TodoTask::sortByDescAsc;
              // description
              break;
            }
            case 1: {
              // date

              sortFunc = todoCore::TodoTask::sortByDateAsc;
              break;
            }
            case 2: {
              // urgency
              sortFunc = todoCore::TodoTask::sortByUrgencyDesc;
              break;
            }
            default: {
              sortFunc = todoCore::TodoTask::sortByStatus;
            }
            }
            updateTasksView(workspaceSelected, 0);
          };
          auto sortMenu =
              ftxui::Menu(&sortOptions, &maintaskSelected, Menuoption);

          tasksWindow->Add(sortMenu);

        }

        else if (workspacePanel->Focused()) {
          // TODO: sort workspaces
        }
        StatusLineMode = "SORT";
        return true;
      }
      if (event == ftxui::Event::Character('q')) {
        // quit
        //
        //
        //
        saveData();
        quit();
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
      StatusLineMode = "NORMAL";
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
      StatusLineMode = "NORMAL";
      saveData();
    };

    newInput = Input(&taskDesc, option);

    tasksWindow->DetachAllChildren();

    for (size_t i = 0; i <= TasksList.size(); i++) {
      if (taskIndex == i) {
        tasksWindow->Add(newInput);
        continue;
      }
      if (i < wpTasks.size()) {
        addToTaskWindow(TasksList[i]);
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
    auto statusLineView =
        todoTransformer.StatusLineRender(statusLine, StatusLineMode);
    return ftxui::vbox({mainView | ftxui::flex,
                        statusLineView |
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
  void updateStatusLineView(int index) {
    statusLine->DetachAllChildren();
    auto comp = ftxui::Renderer([&] { return ftxui::text(""); });

    statusLine->Add(comp);
    tasksWindow->TakeFocus(); // NOTE: remove this
    if (index >= 0 && index < tasksWindow->ChildCount())
      tasksWindow->SetActiveChild(tasksWindow->ChildAt(index));
  }
  void displayDashBoard() {

    tasksWindow->DetachAllChildren();

    ftxui::MenuEntryOption option;
    option.transform = todoTransformer.StyleDashboard(workspaces);
    tasksWindow->Add(ftxui::MenuEntry("Here a Dashboard", option));

    workspacePanel->TakeFocus();
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
    //

    if (showDashBoard) {
      displayDashBoard();
      return;
    }
    UpdateTasksList(workspaceIndex);
    tasksWindow->DetachAllChildren();
    /* if (workspaceIndex < (int)workspaces.size()) { */
    /*   sortTasks(workspaces[workspaceIndex].tasks, sortFunc); */
    /**/
    /*   for (auto &task : workspaces[workspaceIndex].tasks) { */
    /*     task.updateStatus(); */
    /*     addToTaskWindow(task); */
    /*   } */
    /* } */
    for (auto &task : TasksList) {
      task.updateStatus();
      addToTaskWindow(task);
    }
    if (tasksWindow->ChildCount() == 0) {

      defaultTasksOption.transform = todoTransformer.StyleEmptyTask();
      tasksWindow->Add(ftxui::MenuEntry("Add New Task", defaultTasksOption));
    }
    if (taskIndex >= 0)
      tasksWindow->SetActiveChild(
          tasksWindow->ChildAt(taskIndex)); // WARNING: why -1?
    else
      tasksWindow->SetActiveChild(tasksWindow->ChildAt(0));
  }
  void updateAllView(int workspaceIndex, int taskIndex) {
    updateWorkplaceView(workspaceIndex);
    updateTasksView(workspaceIndex, taskIndex);
    updateStatusLineView(taskIndex);
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
      StatusLineMode = "NORMAL";
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
      StatusLineMode = "NORMAL";
      saveData();
    };

    newInput = ftxui::Input(&taskDesc, option);

    tasksWindow->DetachAllChildren();

    std::vector<todoCore::TodoTask> &workspaceTasks =
        workspaces[workspaceIndex].tasks; // WARNING: maybe not

    size_t size = workspaceTasks.size();
    for (size_t i = 0; i <= size; i++) {
      if (taskSelected == i || workspaceTasks.size() == 0) {
        // insert input element here
        tasksWindow->Add(newInput);
      }
      if (i < TasksList.size()) {
        addToTaskWindow(TasksList[i]);
      }
    }
    isInputState = true;
    taskDesc = ""; //"Task " + std::to_string(size + 1);
    newInput->TakeFocus();
  }
  // adds a task to the taskWIndow
  void addToTaskWindow(todoCore::TodoTask &task) {
    ftxui::MenuEntryOption entry_option;
    task.updateStatus();
    entry_option.transform = todoTransformer.StyleTask(task);
    tasksWindow->Add(ftxui::MenuEntry(task.desc, entry_option));
  }
  void addToWorkspacePanel(todoCore::Workspace &wp) {
    ftxui::MenuEntryOption entry_option;
    entry_option.transform = todoTransformer.StyleWorkspace(wp, true); // BUG:
    workspacePanel->Add(ftxui::MenuEntry(wp.name, entry_option));
  }
  void UpdateTasksList(int workspaceIndex) {
    if (workspaceIndex < (int)workspaces.size()) {
      TasksList.clear();

      sortTasks(workspaces[workspaceIndex].tasks, sortFunc);

      if (searchQuery == "") {
        TasksList.resize(workspaces[workspaceIndex].tasks.size());
        for (size_t i = 0; i < TasksList.size(); i++) {

          TasksList[i] = workspaces[workspaceIndex].tasks[i];
        }
      } else {
        // searching using searchQuery
        //
        //
        for (auto task : workspaces[workspaceIndex].tasks) {
          try {
            std::regex pattern(searchQuery);

            if (std::regex_search(task.desc, pattern)) {
              TasksList.push_back(task);
            }
          } catch (const std::regex_error &e) {
            searchQuery = "";
            UpdateTasksList(workspaceIndex);
          }
        }
      }
      /* TasksList = workspaces[workspaceIndex].tasks; */
      // sorting tasks

      //
    }
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
    sortFunc = todoCore::TodoTask::sortByStatus;
    auto Maincomp = ftxui::Container::Horizontal({workspacePanel, tasksWindow});
    /* loadTasks(); */
    /* defaultWorkspaceOption.transform = defaultWorkspaceStyle; */
    auto comp = ftxui::Container::Vertical({Maincomp, statusLine});

    loadData();
    updateAllView(workspaceSelected, maintaskSelected);

    Add(comp);
  }
  void setExitFunction(std::function<void()> q) { quit = std::move(q); }
};

#endif
