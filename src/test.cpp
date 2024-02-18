#include "../include/todoUi.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iostream>
int main(void) {

  auto rend = ftxui::Make<todoUi>();

  auto scr = ftxui::ScreenInteractive::Fullscreen();

  scr.Loop(rend);
  std::cout << "Hello Cruel World";
  return 0;
}
