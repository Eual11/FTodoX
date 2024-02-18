#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>

using namespace ftxui;

int main(void) {

  bool selected = false;
  auto inner_element = Checkbox("Checkbox", &selected);
  auto collabsible = Collapsible("Show more", inner_element);
  auto scr = ScreenInteractive::TerminalOutput();

  scr.Loop(collabsible);
  return 0;
}
