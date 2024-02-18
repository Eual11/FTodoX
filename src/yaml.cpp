#include "../include/todoCore.hpp"
#include <fstream>
#include <iostream>
#include <yaml-cpp/emitter.h>
#include <yaml-cpp/emittermanip.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>

int main(void) {

  YAML::Emitter emitter;
  std::vector<std::string> workspaceNames = {"Work", "School", "Art", "Gaming"};
  std::vector<todoCore::Workspace> workspaces;
  int i = 0;
  for (auto d : workspaceNames) {
    todoCore::Workspace wp(i, d);
    for (int j = 0; j < 5; j++)
      wp.tasks.push_back(
          todoCore::TodoTask(i, "Task " + std::to_string(j * i)));
    workspaces.push_back(std::move(wp));
    ;
    i++;
  }

  emitter << YAML::BeginSeq;

  for (auto wp : workspaces) {
    emitter << wp;
  }
  emitter << YAML::EndSeq;

  std::ofstream out("../test.yaml");
  out << emitter.c_str();

  std::cout << "Hello Cruel World\n";

  return 0;
}
