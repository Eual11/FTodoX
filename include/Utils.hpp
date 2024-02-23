#ifndef _FTXODO_UTILS
#define _FTXODO_UTILS
#include <algorithm>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
ftxui::Color hexToRGB(std::string hex) {

  int i = 0;
  uint32_t rgb = 0x00;
  for (std::string::reverse_iterator iter = hex.rbegin(); iter != hex.rend();
       iter++, i++) {
    unsigned int val = 0;
    if (std::isalpha(*iter) && std::tolower(*iter) >= 'a' &&
        std::tolower(*iter) <= 'f') {

      val = 10 + std::tolower(*iter) - 'a';

    } else if (std::tolower(*iter) >= '0' && std::tolower(*iter) <= '9') {
      val = *iter - '0';
    } else
      val = 0;
    rgb += val * std::pow(16, i);
  }
  return ftxui::Color(rgb >> 16, (rgb & 0xff00) >> 8, rgb & 0xff);
  /* return rgb >> 8 & 0x00ff00; */
}
#endif
