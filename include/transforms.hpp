#ifndef _STYLE_TRANSFORMS_HPP
#define _STYLE_TRANSFORMS_HPP
#include "Utils.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

// NOTE: I THINK THIS SHOULD BE A CLASS WITH IT'S OWN NAME SPACE

// varuable naning state (selected, type, task/workspace)
//  colors

// varuable naning state (selected, type, task/workspace)
//  colors
auto todoBorderStyle = ftxui::borderLight;
; // border style
auto statusLineBGcolor = ftxui::bgcolor(hexToRGB("#8bcd5b"));
auto statusLineColor = ftxui::color(hexToRGB("#141018"));
// transform for selcted workspaces =

// colors
auto inputTextColor = ftxui::color(hexToRGB("#9fd1fa"));

auto selectedTaskBGcolor = ftxui::bgcolor(hexToRGB("#373e4f"));
auto selectedWorkspaceBGcolor = ftxui::bgcolor(hexToRGB("#373e4f"));
auto workspaceColor =
    color(hexToRGB("#93a4c3")); // text color for non selecte workspace
auto selectedWorkspaceColor = color(hexToRGB("#93a4c3"));
auto pendingTaskColor = ftxui::color(hexToRGB("#ebcb8b"));
auto pendingTaskBackground = ftxui::bgcolor(hexToRGB("#373e4f"));
auto completedTaskColor = ftxui::color(hexToRGB("#98c379"));
auto compeletedTaskBackground = ftxui::bgcolor(hexToRGB("#373e4f"));

std::string todobgColor = "#21283b"; // default bavground color for the app
// icons
std::string workspacePointer = ""; // the pointer in workspace;
std::string taskPointer = "";
std::string nonEmptyWorkspaceIcon = "";
std::string pendingTaskIcon = "";
std::string completedTaskIcon = "";
ftxui::Element selectedWorkspaceStyle(const ftxui::EntryState &state) {

  std::string label = workspacePointer + " " + nonEmptyWorkspaceIcon + " ";
  return ftxui::hbox({ftxui::text(label), ftxui::text(state.label)}) |
         selectedWorkspaceBGcolor | selectedWorkspaceColor;
}

// default style for all workplace entries
ftxui::Element defaultWorkspaceStyle(const ftxui::EntryState &state) {
  std::string label =
      (state.focused ? workspacePointer + " " + nonEmptyWorkspaceIcon + " "
                     : "  " + nonEmptyWorkspaceIcon + " ");

  ftxui::Element entry =
      ftxui::hbox({ftxui::text(label), ftxui::text(state.label)}) |
      ftxui::xflex_grow | workspaceColor;

  if (state.active) {
    entry |= ftxui::bold;
  }

  return entry;
}

// default style for todo tasks
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
      pendingTaskColor;
  ;
  if (state.active) {
    entry |= ftxui::bold | pendingTaskColor; // NOTE: please remove this
    ;
  }
  if (state.focused) {
    entry |= ftxui::bold | pendingTaskBackground;
  }

  return entry;
}

// default style for completed to do tasks
ftxui::Element defaultCompletedTaskStyle(const ftxui::EntryState &state) {
  /* ftxui::Element entry = ftxui::text(std::move(label)); */
  std::string icon;
  if (state.focused)
    icon = taskPointer + " " + completedTaskIcon + " ";
  else
    icon = "  " + completedTaskIcon + " ";
  ftxui::Element entry =
      ftxui::hbox({ftxui::text(icon), ftxui::text(state.label)}) |
      completedTaskColor;
  if (state.active) {
    entry |= ftxui::bold | completedTaskColor;
    ;
    ;
  }
  if (state.focused) {
    entry |= ftxui::bold | compeletedTaskBackground;
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
                         selectedTaskBGcolor | inputTextColor;
  return entry;
}

ftxui::Element defaultWorkspaceInputStyle(ftxui::InputState state) {

  std::string icon;
  if (state.is_placeholder)
    state.element |= ftxui::dim;
  if (state.focused)
    icon = workspacePointer + " " + nonEmptyWorkspaceIcon + " ";
  else
    icon = "  " + nonEmptyWorkspaceIcon;
  ftxui::Element entry = ftxui::hbox({ftxui::text(icon), state.element}) |
                         selectedWorkspaceBGcolor | inputTextColor;
  return entry;
}
#endif
