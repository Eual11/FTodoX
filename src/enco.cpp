#include <codecvt>
#include <iostream>
#include <locale>
#include <string>
int main(void) {

  std::wstring myString = L"😀";
  std::wstring_convert<std::codecvt_utf16<wchar_t>> conver;

  std::string newStr = conver.to_bytes(myString);

  std::cout << newStr << std::endl;
  return 0;
}
