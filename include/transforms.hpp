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
std::string todobgColor = "#21283b"; // default bavground color for the app
auto todoBorderStyle = ftxui::borderLight; // border style
auto statusLineBGcolor = ftxui::bgcolor(hexToRGB("#8bcd5b"));
auto statusLineColor = ftxui::color(hexToRGB("#141018"));
// transform for selcted workspaces
ftxui::Element selectedWorkspaceStyle(const ftxui::EntryState &state) {
  std::string label = "  ";
  return ftxui::hbox({ftxui::text(label), ftxui::text(state.label)}) |
         ftxui::bgcolor(ftxui::Color::RGB(55, 62, 79));
}

// default style for all workplace entries
ftxui::Element defaultWorkspaceStyle(const ftxui::EntryState &state) {
  std::string label = (state.focused ? "  " : "   ");
  auto workspaceColor = color(hexToRGB("#93a4c3"));
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

  auto completedTaskColor = ftxui::color(hexToRGB("#ebcb8b"));
  auto compeletedTaskBackground = ftxui::bgcolor(hexToRGB("#373e4f"));

  std::string pointer = "";
  std::string checkbox = " ";
  if (state.focused)
    checkbox = pointer + " " + checkbox;
  else
    checkbox = "  " + checkbox;
  ftxui::Element entry =
      ftxui::hbox({ftxui::text(checkbox), ftxui::text(state.label)}) |
      completedTaskColor;
  ;
  if (state.active) {
    entry |= ftxui::bold | compeletedTaskBackground; // NOTE: please remove this
    ;
  }
  if (state.focused) {
    entry |= ftxui::bold | compeletedTaskBackground;
  }

  return entry;
}

// default style for completed to do tasks
ftxui::Element defaultCompletedTaskStyle(const ftxui::EntryState &state) {
  std::string label =
      state.focused ? ">  " + state.label : "  " + state.label;
  /* ftxui::Element entry = ftxui::text(std::move(label)); */
  std::string pointer = "";
  std::string checkbox = " ";
  auto completedTaskColor = ftxui::color(hexToRGB("#98c379"));
  auto compeletedTaskBackground = ftxui::bgcolor(hexToRGB("#373e4f"));
  if (state.focused)
    checkbox = pointer + " " + checkbox;
  else
    checkbox = "  " + checkbox;
  ftxui::Element entry =
      ftxui::hbox({ftxui::text(checkbox), ftxui::text(state.label)}) |
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
#endif
