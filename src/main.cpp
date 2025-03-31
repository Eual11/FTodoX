#include "../include/todoUi.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
using namespace ftxui;
Component HelpUI() {
  return Renderer([] {
    return vbox({

               filler() | size(ftxui::HEIGHT, ftxui::EQUAL, 1),
               ftxui::hbox({filler(), ftxui::text("Welcome to FTodoX") | center,
                            filler()}) |
                   borderLight |
                   ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 80) | center,

               filler() | size(ftxui::HEIGHT, ftxui::EQUAL, 1),
               hbox({filler(),
                     text("Key Bindings") | bgcolor(hexToRGB("#809be5")),
                     filler()}),
               filler() | size(ftxui::HEIGHT, ftxui::EQUAL, 1),
               text(" a      -> Add a new task or workspace"),
               text(" x      -> Delete a task or workspace"),
               text(" r      -> Edit the workspace name or task description"),
               text(" h      -> Focus on the workspaces list"),
               text(" l      -> Focus on the tasks list"),
               text(" k      -> Move the cursor up in the focused list"),
               text(" j      -> Move the cursor down in the focused list"),
               text(" Enter  -> Confirm the current action or input"),
               text(" /      -> Search for tasks by their description"),
               text(" +      -> Increase task urgency"),
               text(" -      -> Decrease task urgency"),
               text(" s      -> Open the Sort Tasks menu"),
               text(" ?      -> Toggle the help menu"),
               text(" Escape -> Show the dashboard"),
               text(" q      -> Quit"),
               filler() | size(ftxui::HEIGHT, ftxui::EQUAL, 6),

           }) |
           bgcolor(hexToRGB("#21283b")) | center |
           ftxui::size(ftxui::HEIGHT, GREATER_THAN, 80);
  });
}

int main(void) {

  auto todoui = ftxui::Make<todoUi>();

  bool show_help = false;
  auto scr = ftxui::ScreenInteractive::Fullscreen();
  auto exitFunc = scr.ExitLoopClosure();
  auto help = HelpUI();
  todoui->setExitFunction(exitFunc);

  auto layout = ftxui::CatchEvent(todoui, [&](ftxui::Event event) {
    if (event.is_character() and event.character() == "?") {
      show_help = !show_help;
      return true;
    }
    return todoui->OnEvent(event);
  });

  auto TODOUI = ftxui::CatchEvent(
      ftxui::Renderer(
          [&]() { return show_help ? help->Render() : layout->Render(); }),
      [&](ftxui::Event event) { return layout->OnEvent(event); });
  scr.Loop(TODOUI);

  return 0;
}
