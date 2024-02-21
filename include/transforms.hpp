#ifndef _STYLE_TRANSFORMS_HPP
#define _STYLE_TRANSFORMS_HPP
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

// transform for selcted workspaces
ftxui::Element selectedWorkspaceStyle(const ftxui::EntryState &state) {
  std::string label = "> " + state.label;

  return ftxui::text(std::move(label)) | ftxui::color(ftxui::Color::Green);
}

// default style for all workplace entries
ftxui::Element defaultWorkspaceStyle(const ftxui::EntryState &state) {
  std::string label = (state.focused ? " >" : " ") + state.label;
  ftxui::Element entry = ftxui::text(label);
  if (state.active) {
    entry |= ftxui::bold;
  }

  return entry;
}

// default style for checked todo entries

ftxui::Element defaultTaskStyle(const ftxui::EntryState &state) {
  std::string label =
      state.focused ? "> ( ) " + state.label : " ( ) " + state.label;
  ftxui::Element entry = ftxui::text(std::move(label));

  if (state.active) {
    entry |= ftxui::color(ftxui::Color::Red);
    ;
  }
  if (state.focused) {
    entry |= ftxui::bold;
  }

  return entry;
}
ftxui::Element defaultCompletedTaskStyle(const ftxui::EntryState &state) {
  std::string label =
      state.focused ? "> (x) " + state.label : " (x) " + state.label;
  ftxui::Element entry = ftxui::text(std::move(label));

  if (state.active) {
    entry |= ftxui::color(ftxui::Color::Red);
    ;
  }
  if (state.focused) {
    entry |= ftxui::bold;
  }

  return entry;
}
#endif
