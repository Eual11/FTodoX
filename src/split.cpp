#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

int main(void) {

  auto left = Renderer([] { return text("Left") | center; });
  auto right = Renderer([] { return text("Right") | center; });
  auto middle = Renderer([] { return text("Middle") | center; });
  auto top = Renderer([] { return text("Top") | center; });
  auto bottom = Renderer([] { return text("Bottom") | center; });
  int left_size = 10;
  int right_size = 10;
  int top_size = 10;
  int bottom_size = 10;
  auto container = middle;

  container = ResizableSplitLeft(left, container, &left_size);
  container = ResizableSplitRight(right, container, &right_size);
  container = ResizableSplitTop(top, container, &top_size);
  container = ResizableSplitBottom(bottom, container, &bottom_size);
  auto render =
      Renderer(container, [&] { return container->Render() | border; });
  auto scr = ScreenInteractive::Fullscreen();
  scr.Loop(render);
  return 0;
}
