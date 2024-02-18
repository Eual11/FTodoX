#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <string>
#include <vector>
using namespace ftxui;

int main(void) {

  std::vector<std::string> entries = {
      "tribute",     "clearance", "ally",        "bend",        "electronics",
      "module",      "era",       "cultural",    "sniff",       "nationalism",
      "negotiation", "deliver",   "figure",      "east",        "tribute",
      "clearance",   "ally",      "bend",        "electronics", "module",
      "era",         "cultural",  "sniff",       "nationalism", "negotiation",
      "deliver",     "figure",    "east",        "tribute",     "clearance",
      "ally",        "bend",      "electronics", "module",      "era",
      "cultural",    "sniff",     "nationalism", "negotiation", "deliver",
      "figure",      "east",
  };

  int selected_1 = 0;
  int selected_2 = 2;
  int selected_3 = 6;
  int selected_4 = 5;

  auto dropDownContainer = Container::Vertical({

      Container::Horizontal(
          {Dropdown(&entries, &selected_1), Dropdown(&entries, &selected_2)}),
      Container::Horizontal({

          Dropdown(&entries, &selected_3), Dropdown(&entries, &selected_4)})});

  ScreenInteractive scr = ScreenInteractive::FitComponent();

  scr.Loop(dropDownContainer);

  std::cout << "From Selection 1 " << entries[selected_1] << " was selected\n";
  std::cout << "From Selection 2 " << entries[selected_2] << " was selected\n";
  std::cout << "From Selection 3 " << entries[selected_3] << " was selected\n";
  std::cout << "From Selection 4 " << entries[selected_4] << " was selected\n";
  return 0;
}
