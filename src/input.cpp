#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
using namespace ftxui;

int main(void) {

  std::string firstName;
  std::string lastName;
  std::string password;
  std::string phonNumber;

  auto inputFirstName = Input(&firstName, "First Name");
  auto inputLastName = Input(&lastName, "Last Name");
  InputOption passOption;
  passOption.multiline = false;
  passOption.password = true;

  auto inputPassword = Input(&password, "Password", passOption);
  auto inputPhoneNumber = Input(&phonNumber, "Phone Number");

  Component container = Container::Vertical(
      {inputFirstName, inputLastName, inputPassword, inputPhoneNumber});

  auto inputBlock = Renderer(container, [&]() {
    return vbox({

               hbox({text("Name: "), inputFirstName->Render()}),
               hbox({text("Last Name: "), inputLastName->Render()}),
               hbox({text("Password: "), inputPassword->Render()}),
               hbox({text("Phone Number: "), inputPhoneNumber->Render()}),
               separator(), text("Hello " + firstName + " " + lastName),
               text("Your Password is " + password),
               text("We can call you with " + phonNumber)

           }) |
           border;
  });
  auto scr = ScreenInteractive::TerminalOutput();

  scr.Loop(inputBlock);
  return 0;
}
