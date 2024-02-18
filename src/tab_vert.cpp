#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>
using namespace ftxui;

int main(void) {
  std::vector<std::string> tab_menu = {"Tab 1", "Tab 2", "Tab 3"};

  int menu_selected = 0;

  Component menu = Toggle(&tab_menu, &menu_selected);

  std::vector<std::string> menu_selection_1 = {"Gamer", "Pain", "Omori",
                                               "Failure"};
  int selected_1 = 1;

  std::vector<std::string> menu_selection_2 = {"The",   "Pain",  "Will",
                                               "Never", "Reach", "Me"};
  int selected_2 = 1;

  std::vector<std::string> menu_selection_3 = {"If", "I", "Feel", "Nothing"};
  int selected_3 = 3;

  auto tab_menu_container =
      Container::Tab({Radiobox(&menu_selection_1, &selected_1),
                      Radiobox(&menu_selection_2, &selected_2),
                      Radiobox(&menu_selection_3, &selected_3)},
                     &menu_selected);

  auto container = Container::Vertical({

      menu, tab_menu_container});

  auto render = Renderer(container, [&]() {
    return vbox({menu->Render(), separator(), tab_menu_container->Render()}) |
           border;
  });

  auto scr = ScreenInteractive::TerminalOutput();
  scr.Loop(render);
  return 0;
}
