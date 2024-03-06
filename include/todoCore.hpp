#ifndef _TODO_CORE_HPP
#define _TODO_CORE_HPP
#include <chrono>
#include <corecrt.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <yaml-cpp/emitter.h>
#include <yaml-cpp/emittermanip.h>
#include <yaml-cpp/emitterstyle.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>

const int WORKPALCE_PROPERTY_COUNT = 4;
const int TASK_PROPERTY_COUNT = 5;
// the core for the todo list contains the objects of Workpalce
// and TodoTask
namespace todoCore {
enum class TaskStatus { STARTED, COMPLETED, OVERDUE };
struct TodoTask {
  TodoTask() {
    this->id = 0;
    this->desc = "";
    this->status = TaskStatus::STARTED;
    this->urgency = 1;
    this->dueTime = std::numeric_limits<time_t>::max();
  };

  TodoTask(int id, std::string desc, TaskStatus status = TaskStatus::STARTED,
           std::time_t dueTime = std::numeric_limits<time_t>::max(),
           int urgency = 1) {
    this->id = id;
    this->desc = desc;
    this->status = status; //, struct stat *const Stat)

    this->dueTime = dueTime;
    this->urgency = urgency;
  }

  TodoTask(const TodoTask &tk) {
    this->id = tk.id;
    this->desc = tk.desc;
    this->urgency = tk.urgency;
    this->dueTime = tk.dueTime;
    this->status = tk.status;
  }
  TodoTask(TodoTask &&tk) noexcept {
    this->id = tk.id;
    this->desc = tk.desc;
    this->urgency = tk.urgency;
    this->dueTime = tk.dueTime;
    this->status = tk.status;
  }
  TodoTask &operator=(const TodoTask &tk) {
    if (this != &tk) {
      this->id = tk.id;
      this->desc = tk.desc;
      this->urgency = tk.urgency;
      this->dueTime = tk.dueTime;
      this->status = tk.status;
    }
    return *this;
  }
  ~TodoTask() { desc.clear(); }

  inline bool operator==(const todoCore::TodoTask &tk) const {
    return id == tk.id;
  }
  void setCompleted() { status = TaskStatus::COMPLETED; }
  inline void toggleCompleted() {
    status = (status == TaskStatus::COMPLETED || status == TaskStatus::OVERDUE)
                 ? TaskStatus::STARTED
                 : TaskStatus::COMPLETED;
    dueTime = std::numeric_limits<time_t>::max();
  }
  int id;
  std::string desc;
  TaskStatus status;
  std::time_t dueTime;
  int urgency;
  static bool sortByStatus(const todoCore::TodoTask &a,
                           const todoCore::TodoTask &b) {
    return b.status != todoCore::TaskStatus::STARTED &&
           a.status != todoCore::TaskStatus::COMPLETED;
  }
  static bool sortByDescAsc(const todoCore::TodoTask &a,
                            const todoCore::TodoTask &b) {
    return a.desc < b.desc;
  }
  static bool sortByDateAsc(const todoCore::TodoTask &a,
                            const todoCore::TodoTask &b) {
    return a.dueTime < b.dueTime;
  }
  static bool sortByUrgencyDesc(const todoCore::TodoTask &a,
                                const todoCore::TodoTask &b) {
    return a.urgency > b.urgency;
  }
  std::string getDeadline() {
    if (this->dueTime >= std::numeric_limits<time_t>::max())
      return "";
    std::tm *cal = localtime(&this->dueTime);

    char buffer[80];
    buffer[79] = '\0';
    buffer[0] = '\0';
    int size = std::strftime(buffer, 79, "%b %d %y", cal);
    if (size > 0) {
      std::string dead_line(buffer);
      return dead_line;
    }
    return "";
  }
  inline void increaseUrgency() {
    this->urgency += 1;
    if (this->urgency > 3)
      this->urgency = 3;
  }
  inline void decreaseUrgency() {
    this->urgency -= 1;
    if (this->urgency < 1)
      this->urgency = 1;
  }
  void parseDueDate(std::string date) {

    const char *formats[] = {"%b %d %y", "%m/%d/%y", NULL};
    time_t new_time = -1;
    int i = 0;
    std::stringstream ss(date);
    std::tm cal = {};

    while (new_time < 0 && formats[i]) {
      ss >> std::get_time(&cal, formats[i]);

      new_time = std::mktime(&cal);
      std::cerr << date << " " << new_time << " format: " << formats[i]
                << std::endl;
      i++;
      ss.clear();
      ss.seekg(0);
    }
    if (new_time > 0)
      this->dueTime = new_time;
    bool overdue = updateStatus();
    if (!overdue)
      this->status = TaskStatus::STARTED;
  }
  bool updateStatus(void) {
    if (status == TaskStatus::COMPLETED)
      return false;
    auto currentTime = std::chrono::system_clock::now();

    if (std::chrono::system_clock::to_time_t(currentTime) >= this->dueTime) {
      this->status = TaskStatus::OVERDUE;
      return true;
    } else {
      this->status = TaskStatus::STARTED;
      return false;
    }
  }
  friend std::ostream &operator<<(std::ostream &out, const TodoTask &tk) {

    std::string status;
    switch (tk.status) {
    case TaskStatus::STARTED: {
      status = "STARTED";
      break;
    }
    case TaskStatus::COMPLETED: {

      status = "COMPLETED";
      break;
    }
    case TaskStatus::OVERDUE: {
      status = "OVERDUE";
      break;
    }
    }
    out << "{"
        << "id: " << tk.id << ", desc: " << tk.desc << ", status: " << status
        << "urgency " << tk.urgency << " due: " << tk.dueTime << " }"
        << std::endl;
    return out;
  }

  friend YAML::Emitter &operator<<(YAML::Emitter &out, const TodoTask &tk) {
    std::string status;
    switch (tk.status) {
    case TaskStatus::STARTED: {
      status = "STARTED";
      break;
    }
    case TaskStatus::COMPLETED: {

      status = "COMPLETED";
      break;
    }
    case TaskStatus::OVERDUE: {
      status = "OVERDUE";
      break;
    }
    }

    out << YAML::Flow;
    out << YAML::BeginMap;

    out << YAML::Key << "ID";
    out << YAML::Value;
    out << tk.id;
    out << YAML::Key << "Description";
    out << YAML::Value;
    out << tk.desc;
    out << YAML::Key << "Status";
    out << YAML::Value;
    out << status;
    out << YAML::Key << "DueTime";
    out << YAML::Value;
    out << tk.dueTime;
    out << YAML::Key << "Urgency";
    out << YAML::Value;
    out << tk.urgency;

    out << YAML::EndMap;
    return out;
  }
};

struct Workspace {
  Workspace() {
    id = 0;
    name = "";
    nest = false;
    tasks = {};
  }
  Workspace(int id, std::string workspaceName, bool isNested = false) {
    this->id = id;

    name = workspaceName;
    this->nest = isNested;
    tasks = {};
  }
  Workspace(const Workspace &wp) {
    this->id = wp.id;
    this->name = wp.name;
    this->nest = wp.nest;
    this->tasks = wp.tasks;
  }
  Workspace(Workspace &&wp) noexcept {
    this->id = wp.id;
    this->name = wp.name;
    this->nest = wp.nest;
    this->tasks = std::move(wp.tasks);
  }
  Workspace &operator=(const Workspace &wp) {
    if (this != &wp) {
      this->id = wp.id;
      this->name = wp.name;
      this->nest = wp.nest;
      this->tasks = wp.tasks;
    }
    return *this;
  }
  ~Workspace() {
    name.clear();
    tasks.clear();
  }
  void addTask(int id, std::string desc,
               TaskStatus status = TaskStatus::STARTED) {

    tasks.push_back(TodoTask(id, desc, status));
  }

  int getID() { return this->id; }

  std::string getName() { return this->name; } // NOLINT
  inline bool isNested() { return this->nest; }

  inline void setName(std::string workspacename) { this->name = workspacename; }
  inline void setNested(bool isNested) { this->nest = isNested; }
  inline bool operator==(const todoCore::Workspace &wp) const {
    return this->id == wp.id;
  }
  inline bool operator<(const todoCore::Workspace &wp) const {
    return this->name < wp.name;
  }
  inline bool operator>(const todoCore::Workspace &wp) const {
    return this->name > wp.name;
  }
  int id;
  std::string name;
  bool nest;
  std::vector<todoCore::TodoTask> tasks;

  friend std::ostream &operator<<(std::ostream &out, const Workspace &wp) {
    std::string res = wp.tasks.size() > 0 ? "{...}" : "{}";

    out << "{ Name: " << wp.name << ", Id: " << wp.id
        << ", nested: " << std::boolalpha << wp.nest << ", Tasks: {";
    for (auto d : wp.tasks)
      out << d << "\n";
    out << " }" << std::endl;

    return out;
  }
  friend YAML::Emitter &operator<<(YAML::Emitter &out, const Workspace &wp) {
    std::string res = wp.tasks.size() > 0 ? "{...}" : "{}";
    out << YAML::Flow;
    out << YAML::BeginMap;
    out << YAML::Key << "ID";
    out << YAML::Value;
    out << wp.id;

    out << YAML::Key << "Name";
    out << YAML::Value;
    out << wp.name;

    out << YAML::Key << "Nested";
    out << YAML::YesNoBool;
    out << YAML::Value;
    out << wp.nest;
    out << YAML::Key << "Tasks";
    out << YAML::Value;
    out << wp.tasks;
    out << YAML::EndMap;
    if (!out.good())
      std::cerr << out.GetLastError() << "\n";
    return out;
    // return out;
  }
};

} // namespace todoCore

namespace YAML {

template <> struct convert<todoCore::TodoTask> {

  static YAML::Node encode(const todoCore::TodoTask &task) {
    YAML::Node node;
    std::string status;
    switch (task.status) {
    case todoCore::TaskStatus::STARTED: {
      status = "STARTED";
      break;
    }
    case todoCore::TaskStatus::COMPLETED: {

      status = "COMPLETED";
      break;
    }
    case todoCore::TaskStatus::OVERDUE: {
      status = "OVERDUE";
      break;
    }
    }
    node.SetStyle(YAML::EmitterStyle::Flow);
    node["ID"] = task.id;
    node["Description"] = task.desc;
    node["Status"] = status;
    node["DueTime"] = task.dueTime;
    node["Urgency"] = task.urgency;

    return node;
  }
  // stat( , struct stat *const Stat)

  static bool decode(const YAML::Node &node, todoCore::TodoTask &task) {
    if (!(node.IsMap() && node.size() == TASK_PROPERTY_COUNT)) {
      return false;
    }
    // return false;

    task.id = node["ID"].as<int>();
    task.desc = node["Description"].as<std::string>();
    std::string status = node["Status"].as<std::string>();
    if (!node["DueTime"]) {
      task.dueTime = std::numeric_limits<time_t>::max();
    } else
      task.dueTime = node["DueTime"].as<time_t>();
    if (!node["Urgency"])
      task.urgency = 1;
    else
      task.urgency = node["Urgency"].as<int>();

    if (status == "STARTED")
      task.status = todoCore::TaskStatus::STARTED;
    else if (status == "COMPLETED")
      task.status = todoCore::TaskStatus::COMPLETED;
    else
      task.status = todoCore::TaskStatus::OVERDUE;
    return true;
  }
};

template <> struct convert<todoCore::Workspace> {
  static Node encode(const todoCore::Workspace wp) {
    Node node;
    node.SetStyle(EmitterStyle::Flow);
    node["ID"] = wp.id;
    node["Name"] = wp.name;
    node["Nested"] = wp.nest;

    for (auto d : wp.tasks)
      node["Tasks"].push_back(d);

    return node;
  }
  static bool decode(const Node &node, todoCore::Workspace &wp) {
    if (!(node.IsMap() && node.size() == WORKPALCE_PROPERTY_COUNT))
      return false;

    wp.name = node["Name"].as<std::string>();
    wp.id = node["ID"].as<int>();
    wp.nest = node["Nested"].as<bool>();

    for (auto d : node["Tasks"]) {
      wp.tasks.push_back(d.as<todoCore::TodoTask>());
    }
    return true;
  }

  // naive and simple sorting of tasks;
};

} // namespace YAML
#endif
