#ifndef _TRANSFORMER_HPP_
#define _TRANSFORMER_HPP_
#include "Utils.hpp"
#include "todoCore.hpp"
#include <exception>
#include <fstream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>
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

      node["workspace_pointer"] = workspacePointer;
      node["task_pointer"] = taskPointer;
      node["non_empty_workspace_icon"] = nonEmptyWorkspaceIcon;
      node["empty_workspace_icon"] = emptyWorkspaceIcon;
      node["pending_task_icon"] = pendingTaskIcon;
      node["completed_task_icon"] = completedTaskIcon;

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

    workspacePointer = LoadedTheme["workspace_pointer"].as<std::string>();
    taskPointer = LoadedTheme["task_pointer"].as<std::string>();
    emptyWorkspaceIcon = LoadedTheme["empty_workspace_icon"].as<std::string>();
    nonEmptyWorkspaceIcon =
        LoadedTheme["non_empty_workspace_icon"].as<std::string>();
    pendingTaskIcon = LoadedTheme["pending_task_icon"].as<std::string>();
    completedTaskIcon = LoadedTheme["completed_task_icon"].as<std::string>();
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

  /* std::string todobgColor = "#21283b"; // default bavground color for the app
   */
  // icons
  std::string workspacePointer = ""; // the pointer in workspace;
  std::string taskPointer = "";
  std::string nonEmptyWorkspaceIcon = "";
  std::string emptyWorkspaceIcon = "";
  std::string pendingTaskIcon = "";
  std::string completedTaskIcon = "";
  std::string workspaceIcon;
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
  StyleTask(todoCore::TodoTask task) {
    switch (task.status) {
    case todoCore::TaskStatus::STARTED: {

      return [this](const ftxui::EntryState &state) {
        return defaultTaskStyle(state);
      };
      break;
    }
    case todoCore::TaskStatus::COMPLETED: {
      return [this](const ftxui::EntryState &state) {
        return defaultCompletedTaskStyle(state);
      };
    }
    case todoCore::TaskStatus::OVERDUE: {
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
};
#endif
