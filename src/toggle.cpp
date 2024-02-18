#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <string>
#include <vector>
using namespace ftxui;
int main(void) {

  std::vector<std::string> toggle_1_entries = {"On", "Off"};
  std::vector<std::string> toggle_2_entries = {"Hatred", "Determination"};
  std::vector<std::string> toggle_3_entries = {"gcc", "g++"};
  std::vector<std::string> toggle_4_entries = {"myself", "some", "all"};

  int toggle_1_selected;
  int toggle_2_selected;
  int toggle_3_selected;
  int toggle_4_selected;

  auto toggle1 = Toggle(&toggle_1_entries, &toggle_1_selected);

  auto toggle2 = Toggle(&toggle_2_entries, &toggle_2_selected);
  auto toggle3 = Toggle(&toggle_3_entries, &toggle_3_selected);
  auto toggle4 = Toggle(&toggle_4_entries, &toggle_4_selected);

  auto container = Container::Vertical({toggle1, toggle2, toggle3, toggle4

  });

  auto options = Renderer(container, [&]() {
    return vbox(

        {hbox({text("Emotions: "), toggle1->Render()}),
         hbox({text("Reason to go on: "), toggle2->Render()}),
         hbox({text("Compiler: "), toggle3->Render()}),
         hbox({text("Live for: "), toggle4->Render()})});
  });

  auto scr = ScreenInteractive::TerminalOutput();

  scr.Loop(options);
  return 0;
}
