#include <cctype>
#include <cmath>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <stdint.h>
#include <string>

#define topMargin 1;
ftxui::Color hexToRGB(std::string hex);
using namespace ftxui;

int main(void) {

  auto borderStyle = borderLight;
  auto scr = ScreenInteractive::Fullscreen();

  std::vector<std::string> entries = {"Code", "Hell", "Other stuff"};
  std::string firstName;
  int Selected = 0;
  auto input = Input(&firstName);

  int screeWidth = scr.dimx();

  int screenHeight = scr.dimy();

  int sideBarWidth = screeWidth * 0.2;
  int sideBarHeight = screenHeight - 2;

  auto txt = Renderer([] { return text("Hello"); });
  auto sideBar = Container::Vertical({input, txt});

  // sidebar
  sideBar |=
      Renderer([&](Element inner) { return vbox({inner}) | borderStyle; });

  // main window
  // for rendering the todos,
  // vertical box that aligns elements vertically
  // flex_grow to grow to the xdirection
  auto mainWindow = Renderer([&] {
    return vbox({text("this is a test")}) | center | borderStyle | flex_grow;
  });
  auto statusLine =
      Renderer([&] { return hbox({text("this is a test")}) | borderStyle; });
  auto container = Container::Horizontal({sideBar, mainWindow});
  container = Container::Vertical({container, statusLine});
  auto layOut = Renderer(container, [&] {
    sideBarWidth = scr.dimx() * 0.2;
    sideBarHeight = scr.dimy() - 2;
    sideBarWidth <= 20 ? sideBarWidth = 20 : 35;
    auto mainSide = hbox(
        {sideBar->Render() | size(ftxui::WIDTH, ftxui::EQUAL, sideBarWidth),
         mainWindow->Render()});

    return vbox({mainSide | flex_grow, statusLine->Render()}) |
           bgcolor(hexToRGB("#21283b"));
  });

  scr.Loop(layOut);
  std::cout << "Side bar Width =" << sideBarWidth
            << " Sidebar Height= " << sideBarHeight << std::endl;

  std::cout << "Screen Width =" << scr.dimx()
            << " Screen Height= " << scr.dimy() << std::endl;

  return 0;
}

ftxui::Color hexToRGB(std::string hex) {

  int i = 0;
  uint32_t rgb = 0x00;
  for (std::string::reverse_iterator iter = hex.rbegin(); iter != hex.rend();
       iter++, i++) {
    unsigned int val = 0;
    if (std::isalpha(*iter) && *iter >= 'a' && *iter <= 'f') {

      val = 10 + *iter - 'a';

    } else if (*iter >= '0' && *iter <= '9') {
      val = *iter - '0';
    } else
      val = 0;
    rgb += val * std::pow(16, i);
  }
  return Color(rgb >> 16, (rgb & 0xff00) >> 8, rgb & 0xff);
  /* return rgb >> 8 & 0x00ff00; */
}
