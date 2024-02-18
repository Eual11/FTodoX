#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>

using namespace ftxui;

int main(void) {
  std::vector<bool> selection(100, false);

  auto checkbox_container = Container::Vertical({});

  bool selected = false;
  for (size_t i = 0; i < selection.size(); i++) {

    checkbox_container->Add(
        Checkbox("Checkbox " + std::to_string(i), &selected));
  }

  auto renderer = Renderer(checkbox_container, [&] {
    return checkbox_container->Render() | frame | border |
           size(HEIGHT, LESS_THAN, 10) | vscroll_indicator;
  });
  auto scr = ScreenInteractive::FitComponent();
  renderer |= CatchEvent([&](Event event) {
    if (event == Event::Character('q')) {
      std::cout << "caught";
      scr.ExitLoopClosure();
      return true;
    }
    return false;
  });

  scr.Loop(renderer);
  return 0;
}
