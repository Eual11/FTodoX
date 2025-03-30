#include "../include/todoUi.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
int main(void) {

  auto todoui = ftxui::Make<todoUi>();
  auto scr = ftxui::ScreenInteractive::Fullscreen();
  auto exitFunc =   scr.ExitLoopClosure();
  todoui->setExitFunction(exitFunc);
  scr.Loop(todoui);
  return 0;
}
