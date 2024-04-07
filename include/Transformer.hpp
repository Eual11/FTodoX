#ifndef _TRANSFORMER_HPP_
#define _TRANSFORMER_HPP_
#include "Utils.hpp"
#include "todoCore.hpp"
#include <algorithm>
#include <chrono>
#include <exception>
#include <fstream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>
namespace TaskCount {
static int completed;
static int pending;
static int overdue;
}; // namespace TaskCount
class Transformer {

public:
  bool LoadTheme(std::string themePath) {

    YAML::Node node;
    YAML::Node LoadedTheme;
    try {
      LoadedTheme = YAML::LoadFile(themePath);
    } catch (std::exception e) {

      // something is wrong with the yaml file
      std::cerr << "Couldn't Load " << themePath << "\n"
                << "starting with default theme\n";

      node["name"] = themePath;
      node["background_color"] = todobgColor;
      node["todo_border_style"] = "light";
      node["status_line_color"] = statusLineColor;
      node["selected_task_background_color"] = selectedTaskBGcolor;
      node["selected_workspace_background_color"] = selectedWorkspaceBGcolor;
      node["workspace_color"] = workspaceColor;
      node["selected_workspace_color"] = selectedWorkspaceColor;
      node["pending_task_color"] = pendingTaskColor;
      node["pending_task_background"] = pendingTaskBackground;
      node["completed_task_color"] = completedTaskColor;
      node["completed_task_background"] = completedTaskBackground;
      node["overdue_task_color"] = overdueTaskColor;
      node["overdue_task_background"] = overdueTaskBackground;
      node["urgency_low_color"] = urgencyLowColor;

      node["urgency_medium_color"] = urgencyMediumColor;
      node["urgency_high_color"] = urgencyHighColor;

      node["workspace_pointer"] = workspacePointer;
      node["task_pointer"] = taskPointer;
      node["non_empty_workspace_icon"] = nonEmptyWorkspaceIcon;
      node["empty_workspace_icon"] = emptyWorkspaceIcon;
      node["pending_task_icon"] = pendingTaskIcon;
      node["completed_task_icon"] = completedTaskIcon;
      node["overdue_task_icon"] = overdueTaskIcon;
      node["urgency_low_icon"] = urgencyLowIcon;
      node["urgency_medium_icon"] = urgencyMediumIcon;
      node["urgency_high_icon"] = urgencyHighIcon;
      node["due_time_icon"] = dueTimeIcon;
      node["search_icon"] = searchIcon;
      std::ofstream out("default.yaml");
      if (out.fail()) {
        std::cerr << "couldn't save default theme\n";
        return false;
      } else {
        out << node;

        std::cerr << "\nactuall worked\n";
        return true;
      }
    }

    themeName = LoadedTheme["name"].as<std::string>();
    todobgColor = LoadedTheme["background_color"].as<std::string>();
    statusLineColor = LoadedTheme["status_line_color"].as<std::string>();
    selectedTaskBGcolor =
        LoadedTheme["selected_task_background_color"].as<std::string>();

    workspaceColor = LoadedTheme["workspace_color"].as<std::string>();
    selectedWorkspaceColor =
        LoadedTheme["selected_workspace_color"].as<std::string>();
    pendingTaskColor = LoadedTheme["pending_task_color"].as<std::string>();
    pendingTaskBackground =
        LoadedTheme["pending_task_background"].as<std::string>();
    completedTaskColor = LoadedTheme["completed_task_color"].as<std::string>();
    completedTaskBackground =
        LoadedTheme["completed_task_background"].as<std::string>();
    overdueTaskColor = LoadedTheme["overdue_task_color"].as<std::string>();
    overdueTaskBackground =
        LoadedTheme["overdue_task_background"].as<std::string>();

    urgencyLowColor = LoadedTheme["urgency_low_color"].as<std::string>();
    urgencyMediumColor = LoadedTheme["urgency_medium_color"].as<std::string>();
    urgencyHighColor = LoadedTheme["urgency_high_color"].as<std::string>();

    workspacePointer = LoadedTheme["workspace_pointer"].as<std::string>();
    taskPointer = LoadedTheme["task_pointer"].as<std::string>();
    emptyWorkspaceIcon = LoadedTheme["empty_workspace_icon"].as<std::string>();
    nonEmptyWorkspaceIcon =
        LoadedTheme["non_empty_workspace_icon"].as<std::string>();
    pendingTaskIcon = LoadedTheme["pending_task_icon"].as<std::string>();
    completedTaskIcon = LoadedTheme["completed_task_icon"].as<std::string>();
    overdueTaskIcon = LoadedTheme["overdue_task_icon"].as<std::string>();
    urgencyLowIcon = LoadedTheme["urgency_low_icon"].as<std::string>();
    urgencyMediumIcon = LoadedTheme["urgency_medium_icon"].as<std::string>();
    urgencyHighIcon = LoadedTheme["urgency_high_icon"].as<std::string>();

    dueTimeIcon = LoadedTheme["due_time_icon"].as<std::string>();
    searchIcon = LoadedTheme["search_icon"].as<std::string>();
    return true;
  }

  Transformer(std::string themePath = "default.yaml") {
    // do something based on the path

    bool res = LoadTheme(themePath);
    if (!res) {
      std::cerr << "Couldn't Load theme\n";
    } else {
      std::cerr << "not an error xD\n";
    }
  }

private:
  // styling variables
  ftxui::Decorator todoBorderStyle = ftxui::borderLight;
  // border style
  //  ftxui::Decorator statusLineBGcolor = ftxui::bgcolor(hexToRGB("#8bcd5b"));
  std::string statusLineColor = "#141018";
  // transform for selcted workspaces =

  // colors
  std::string inputTextColor = "#9fd1fa";

  std::string statusLineBGcolor = "#8bcd5b";
  std::string selectedTaskBGcolor = "#373e4f";

  std::string selectedWorkspaceBGcolor = "#373e4f";

  std::string workspaceColor =
      "#93a4c3"; // text color for non selecte workspace
  std::string selectedWorkspaceColor = "#93a4c3";
  std::string pendingTaskColor = "#ebcb8b";
  std::string pendingTaskBackground = "#373e4f";
  std::string completedTaskColor = "#98c379";
  std::string completedTaskBackground = "#373e4f";
  std::string overdueTaskColor = "#bf4a58";
  std::string overdueTaskBackground = "#373e4f";
  std::string urgencyLowColor = "#54e7ac";
  std::string urgencyMediumColor = "#d3e847";
  std::string urgencyHighColor = "#e75462";

  /* std::string todobgColor = "#21283b"; // default bavground color for the app
   */
  // icons
  std::string workspacePointer = ""; // the pointer in workspace;
  std::string taskPointer = "";
  std::string nonEmptyWorkspaceIcon = "";
  std::string emptyWorkspaceIcon = "";
  std::string pendingTaskIcon = "";
  std::string completedTaskIcon = "";
  std::string overdueTaskIcon = "󰅗";

  std::string urgencyLowIcon = "󰿷";
  std::string urgencyMediumIcon = "";
  std::string urgencyHighIcon = "󰚌";
  std::string dueTimeIcon = "";
  std::string searchIcon = "";
  std::string workspaceIcon;
  std::string urgencyIcon;
  ftxui::Decorator urgencyStyle = ftxui::color(hexToRGB(urgencyLowColor));

  ftxui::Element selectedWorkspaceStyle(const ftxui::EntryState &state) {

    std::string label = workspacePointer + " " + workspaceIcon + " ";
    return ftxui::hbox({ftxui::text(label), ftxui::text(state.label)}) |
           ftxui::bgcolor(hexToRGB(selectedWorkspaceBGcolor)) |
           ftxui::color(hexToRGB(selectedWorkspaceColor));
  }

  ftxui::Element defaultWorkspaceStyle(const ftxui::EntryState &state) {
    std::string label =
        (state.focused ? workspacePointer + " " + workspaceIcon + " "
                       : "  " + workspaceIcon + " ");

    ftxui::Element entry =
        ftxui::hbox({ftxui::text(label), ftxui::text(state.label)}) |
        ftxui::xflex_grow | ftxui::color(hexToRGB(workspaceColor));

    if (state.active) {
      entry |= ftxui::bold;
    }

    return entry;
  }

  ftxui::Element EmptyTaskStyle(const ftxui::EntryState &state) {

    std::ifstream ascii_art("./ascii.txt");

    if (ascii_art.fail()) {
      return ftxui::text("fail to load ascii");
    }
    std::string s;
    std::vector<ftxui::Element> entryVec = {};
    while (std::getline(ascii_art, s)) {
      entryVec.push_back(ftxui::text(s));
    }
    entryVec.insert(entryVec.begin(),
                    ftxui::emptyElement() |
                        ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 10));
    entryVec.push_back(ftxui::text(""));

    entryVec.push_back(
        ftxui::text("It's Empty, Add More Tasks and Acheive them ^_^"));
    return ftxui::vbox(entryVec) | ftxui::flex_grow | ftxui::center |
           ftxui::color(hexToRGB("#41a7fc"));
  }
  ftxui::Element DashboardStyle(const ftxui::EntryState &state) {

    std::ifstream ascii_art("./dashboardascii.txt");

    if (ascii_art.fail()) {
      return ftxui::text("fail to load ascii");
    }
    std::string s;
    std::vector<ftxui::Element> entryVec = {};
    while (std::getline(ascii_art, s)) {
      entryVec.push_back(ftxui::text(s));
    }
    entryVec.insert(entryVec.begin(),
                    ftxui::emptyElement() |
                        ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 7));
    entryVec.push_back(ftxui::text(""));
    entryVec.push_back(ftxui::separator());

    auto now =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm *cal = std::localtime(&now);

    std::string date;
    std::ostringstream oss;
    oss << std::put_time(cal, "%A, %m %b");
    date = oss.str();
    entryVec.push_back(ftxui::text(date) |
                       ftxui::color(hexToRGB(pendingTaskColor)) |
                       ftxui::center);
    entryVec.push_back(ftxui::text(""));
    entryVec.push_back(
        ftxui::hbox({ftxui::text(" Tasks Overdue: ") |
                         ftxui::color(hexToRGB(overdueTaskColor)),
                     ftxui::filler(),
                     ftxui::text(std::to_string(TaskCount::overdue))}) |
        ftxui::color(hexToRGB(overdueTaskColor)));
    entryVec.push_back(
        ftxui::hbox({ftxui::text("󱅄 Tasks Pending: ") |
                         ftxui::color(hexToRGB(pendingTaskColor)),
                     ftxui::filler(),
                     ftxui::text(std::to_string(TaskCount::pending))}) |
        ftxui::color(hexToRGB(pendingTaskColor)));
    entryVec.push_back(
        ftxui::hbox({ftxui::text(" Tasks Done: "), ftxui::filler(),
                     ftxui::text(std::to_string(TaskCount::completed))}) |
        ftxui::color(hexToRGB(completedTaskColor)));

    return ftxui::vbox(entryVec) | ftxui::flex_grow | ftxui::center |
           ftxui::color(hexToRGB("#41a7fc"));
  }
  ftxui::Element defaultTaskStyle(const ftxui::EntryState &state) {
    /* ftxui::Element entry = ftxui::text(std::move(label)); Documents and
     * Settings*/

    std::string icon;
    if (state.focused)
      icon = taskPointer + " " + pendingTaskIcon + " ";
    else
      icon = "  " + pendingTaskIcon + " ";
    ftxui::Element entry =
        ftxui::hbox({ftxui::text(icon), ftxui::text(state.label)}) |
        ftxui::color(hexToRGB(pendingTaskColor));
    ;
    if (state.active) {
      entry |=
          ftxui::bold |
          ftxui::color(hexToRGB(pendingTaskColor)); // NOTE: please remove this
      ;
    }
    if (state.focused) {
      entry |= ftxui::bold | ftxui::bgcolor(hexToRGB(pendingTaskBackground));
    }

    return entry;
  }

  void setUrgencyStyle(todoCore::TodoTask &task) {
    switch (task.urgency) {
    case 1: {
      this->urgencyIcon = urgencyLowIcon;

      this->urgencyStyle = ftxui::color(hexToRGB(urgencyLowColor));

      break;
    }
    case 2: {
      urgencyIcon = urgencyMediumIcon;

      urgencyStyle = ftxui::color(hexToRGB(urgencyMediumColor));

      break;
    }
    case 3: {
      urgencyIcon = urgencyHighIcon;

      urgencyStyle = ftxui::color(hexToRGB(urgencyHighColor));

      break;
    }
    }
  }

  ftxui::Element defaultCompletedTaskStyle(const ftxui::EntryState &state) {
    /* ftxui::Element entry = ftxui::text(std::move(label)); */
    std::string icon;
    if (state.focused)
      icon = taskPointer + " " + completedTaskIcon + " ";
    else
      icon = "  " + completedTaskIcon + " ";
    ftxui::Element entry =
        ftxui::hbox({ftxui::text(icon), ftxui::text(state.label)}) |
        ftxui::color(hexToRGB(completedTaskColor));
    if (state.active) {
      entry |= ftxui::bold | ftxui::color(hexToRGB(completedTaskColor));
      ;
      ;
    }
    if (state.focused) {
      entry |= ftxui::bold | ftxui::bgcolor(hexToRGB(completedTaskBackground));
      ;
    }

    return entry;
  }
  ftxui::Element defaultOverDueTaskStyle(const ftxui::EntryState &state) {
    /* ftxui::Element entry = ftxui::text(std::move(label)); */
    std::string icon;
    if (state.focused)
      icon = taskPointer + " " + overdueTaskIcon + " ";
    else
      icon = "  " + overdueTaskIcon + " ";
    ftxui::Element entry =
        ftxui::hbox({ftxui::text(icon), ftxui::text(state.label)}) |
        ftxui::color(hexToRGB(overdueTaskColor));
    if (state.active) {
      entry |= ftxui::bold | ftxui::color(hexToRGB(overdueTaskColor));
      ;
      ;
    }
    if (state.focused) {
      entry |= ftxui::bold | ftxui::bgcolor(hexToRGB(overdueTaskBackground));
      ;
    }

    return entry;
  }

  ftxui::Element defaultTaskInputStyle(ftxui::InputState state) {

    std::string pointer;
    ;
    std::string checkbox;
    if (state.is_placeholder)
      state.element |= ftxui::dim;
    if (state.focused)
      checkbox = taskPointer + " " + pendingTaskIcon + " ";
    else
      checkbox = "  " + pendingTaskIcon + " ";
    ftxui::Element entry = ftxui::hbox({ftxui::text(checkbox), state.element}) |
                           ftxui::bgcolor(hexToRGB(selectedTaskBGcolor)) |
                           ftxui::color(hexToRGB(inputTextColor));
    return entry;
  }
  ftxui::Element defaultWorkspaceInputStyle(ftxui::InputState state) {

    std::string icon;
    if (state.is_placeholder)
      state.element |= ftxui::dim;
    if (state.focused)
      icon = workspacePointer + " " + workspaceIcon + " ";
    else
      icon = "  " + workspaceIcon;
    ftxui::Element entry = ftxui::hbox({ftxui::text(icon), state.element}) |
                           ftxui::bgcolor(hexToRGB(selectedWorkspaceBGcolor)) |
                           ftxui::color(hexToRGB(inputTextColor));
    return entry;
  }
  ftxui::Element defaultStatusLineInputStyle(ftxui::InputState &state) {
    std::string icon;
    if (state.is_placeholder)
      state.element |= ftxui::dim;
    if (state.focused)
      icon = workspacePointer + " " + dueTimeIcon + " ";
    else
      icon = "  " + dueTimeIcon;
    ftxui::Element entry =
        ftxui::hbox(
            {ftxui::text(icon), state.element | ftxui::focusCursorUnderline}) |
        ftxui::bgcolor(hexToRGB(selectedWorkspaceBGcolor)) |
        ftxui::color(hexToRGB(inputTextColor));
    return entry;
  }
  ftxui::Element defaultStatusLineSearchInputStyle(ftxui::InputState &state) {
    std::string icon;
    if (state.is_placeholder)
      state.element |= ftxui::dim;
    if (state.focused)
      icon = " " + searchIcon + " ";
    else
      icon = "  " + searchIcon;
    ftxui::Element entry =
        ftxui::hbox(
            {ftxui::text(icon), state.element | ftxui::focusCursorUnderline}) |
        ftxui::bgcolor(hexToRGB(selectedWorkspaceBGcolor)) |
        ftxui::color(hexToRGB(inputTextColor));
    return entry;
  }

public:
  std::string todobgColor = "#21283b";
  std::string themeName;
  std::function<ftxui::Element(const ftxui::EntryState &)>
  StyleWorkspace(todoCore::Workspace &wp, bool selected) {

    if (selected)
      return [&](const ftxui::EntryState &state) {
        if (wp.tasks.size() == 0) {
          workspaceIcon = emptyWorkspaceIcon;
        } else
          workspaceIcon = nonEmptyWorkspaceIcon;
        return selectedWorkspaceStyle(state);
      };
    else {
      return [&](const ftxui::EntryState &state) {
        if (wp.tasks.size() == 0) {
          workspaceIcon = emptyWorkspaceIcon;
        } else
          workspaceIcon = nonEmptyWorkspaceIcon;
        return defaultWorkspaceStyle(state);
      };
    }
  }

  std::function<ftxui::Element(const ftxui::InputState &)>
  StyleWorkspaceInput(todoCore::Workspace &wp) {
    return [&](const ftxui::InputState &state) {
      if (wp.tasks.size() == 0) {
        workspaceIcon = emptyWorkspaceIcon;
      } else
        workspaceIcon = nonEmptyWorkspaceIcon;
      return defaultWorkspaceInputStyle(state);
    };
  }
  std::function<ftxui::Element(const ftxui::EntryState &)>
  StyleTask(todoCore::TodoTask &task) {
    switch (task.status) {
    case todoCore::TaskStatus::STARTED: {

      return [&](const ftxui::EntryState &state) {
        setUrgencyStyle(task);
        std::string dueString = task.getDeadline();
        ftxui::Element due = ftxui::text(dueTimeIcon + " " + dueString) |
                             ftxui::color(hexToRGB(pendingTaskColor)) |
                             ftxui::color(hexToRGB(pendingTaskBackground));
        if (!dueString.size())
          due = ftxui::text("");
        auto entry =
            ftxui::hbox({defaultTaskStyle(state), ftxui::filler(), due,
                         ftxui::emptyElement() |
                             ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 10),
                         ftxui::text(this->urgencyIcon) | this->urgencyStyle});
        if (state.focused) {
          entry |=
              ftxui::bold | ftxui::bgcolor(hexToRGB(completedTaskBackground));
        }
        return entry;
      };
      break;
    }
    case todoCore::TaskStatus::COMPLETED: {
      return [&](const ftxui::EntryState &state) {
        setUrgencyStyle(task);
        std::string dueString = task.getDeadline();
        ftxui::Element due = ftxui::text(dueTimeIcon + " " + dueString) |
                             ftxui::color(hexToRGB(completedTaskColor)) |
                             ftxui::color(hexToRGB(completedTaskBackground));
        ;
        if (!dueString.size())
          due = ftxui::text("");
        auto entry =
            ftxui::hbox({defaultCompletedTaskStyle(state), ftxui::filler(), due,
                         ftxui::emptyElement() |
                             ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 10),
                         ftxui::text(urgencyIcon) | urgencyStyle});
        if (state.focused) {
          entry |=
              ftxui::bold | ftxui::bgcolor(hexToRGB(completedTaskBackground));
        }
        return entry;
      };
    }
    case todoCore::TaskStatus::OVERDUE: {
      return [&](const ftxui::EntryState &state) {
        setUrgencyStyle(task);
        std::string dueString = task.getDeadline();
        ftxui::Element due = ftxui::text(dueTimeIcon + " " + dueString) |
                             ftxui::color(hexToRGB(overdueTaskColor)) |
                             ftxui::color(hexToRGB(overdueTaskBackground));

        if (!dueString.size())
          due = ftxui::text("");
        auto entry =
            ftxui::hbox({defaultOverDueTaskStyle(state), ftxui::filler(), due,
                         ftxui::emptyElement() |
                             ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 10),
                         ftxui::text(urgencyIcon) | urgencyStyle});
        if (state.focused) {
          entry |=
              ftxui::bold | ftxui::bgcolor(hexToRGB(completedTaskBackground));
        }
        return entry;
      };
      break;
    }
    };
    return {};
  }

  std::function<ftxui::Element(const ftxui::InputState &)>
  StyleTaskInput(todoCore::TodoTask task) {
    return [this](const ftxui::InputState &state) {
      return defaultTaskInputStyle(state);
    };
  }
  std::function<ftxui::Element(ftxui::InputState)>
  StyleStatusLineInput(std::string type) {
    if (type == "date") {
      return [this](ftxui::InputState state) {
        return defaultStatusLineInputStyle(state);
      };
    } else {
      return [this](ftxui::InputState state) {
        return defaultStatusLineSearchInputStyle(state);
      };
    }
  }

  std::function<ftxui::Element(const ftxui::EntryState &)> StyleEmptyTask() {
    return [this](const ftxui::EntryState &state) {
      return EmptyTaskStyle(state);
    };
  }
  std::function<ftxui::Element(const ftxui::EntryState &)>
  StyleDashboard(const std::vector<todoCore::Workspace> &wps) {

    TaskCount::completed = 0;
    TaskCount::pending = 0;
    TaskCount::overdue = 0;
    for (auto &wp : wps) {
      TaskCount::completed += std::count_if(
          wp.tasks.begin(), wp.tasks.end(), [](const todoCore::TodoTask &tk) {
            return tk.status == todoCore::TaskStatus::COMPLETED;
          });
      TaskCount::overdue += std::count_if(
          wp.tasks.begin(), wp.tasks.end(), [](const todoCore::TodoTask &tk) {
            return tk.status == todoCore::TaskStatus::OVERDUE;
          });
      TaskCount::pending += std::count_if(
          wp.tasks.begin(), wp.tasks.end(), [](const todoCore::TodoTask &tk) {
            return tk.status == todoCore::TaskStatus::STARTED;
          });
    }

    return [this](const ftxui::EntryState &state) {
      return DashboardStyle(state);
    };
  }

  ftxui::Element StatusLineRender(ftxui::Component statusLine,
                                  std::string statusLineMode) {
#ifdef _WIN32

    const char *username = getenv("USERNAME");

#elif __APPLE__
    const char *username = getenv("LOGNAME");

#elif __linux__
    const char *username = getenv("USER")

#else
    const char *username = nullptr;
#endif
    std::string user;
    if (username)
      user = username;

    using SC = std::chrono::system_clock;

    auto now = SC::to_time_t(SC::now());

    std::tm *cal = std::localtime(&now);
    std::string hours = std::to_string(cal->tm_hour);
    std::string mins = cal->tm_min <= 9 ? "0" + std::to_string(cal->tm_min)
                                        : std::to_string(cal->tm_min);

    std::string current_time = "  " + hours + ":" + mins + "  ";
    auto statusLineView = ftxui::hbox(
        {ftxui::text("  " + statusLineMode) |
             ftxui::color(hexToRGB(statusLineColor)) |
             ftxui::bgcolor(hexToRGB(statusLineBGcolor)),
         statusLine->Render(), ftxui::filler(),
         ftxui::text(user + "   ") |
             ftxui::color(hexToRGB(statusLineColor)) |
             ftxui::bgcolor(hexToRGB(statusLineBGcolor)),
         ftxui::text(current_time) | ftxui::color(hexToRGB(statusLineColor)) |
             ftxui::bgcolor(hexToRGB(statusLineBGcolor))});

    return statusLineView;
  }
};
#endif
