#ifndef TRANSACTIONS_SOURCE_VIEW_BASEVIEW_H_
#define TRANSACTIONS_SOURCE_VIEW_BASEVIEW_H_

#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>

#include "model/parser/parser.h"

namespace s21 {
class BaseView {
 public:
  struct MenuAction {
    std::function<void()> function;
    std::string description;
  };

  virtual void Start() = 0;

 protected:
  std::stack<std::map<std::string, MenuAction>> stack_menu_;
  Parser parser_;

  void PushMenu(const std::map<std::string, MenuAction>& menu) {
    stack_menu_.push(menu);
  }

  void PopMenu() {
    stack_menu_.pop();
    if (!stack_menu_.empty()) {
      DisplayMenu(stack_menu_.top());
    }
  }

  void DisplayMenu(const std::map<std::string, MenuAction>& menu) {
    for (auto elem : menu) {
      std::cout << elem.first << " " << elem.second.description << std::endl;
    }
  }

  void ExecuteCommand(const std::string& command) {
    try {
      auto callable = stack_menu_.top().find(command);
      if (callable != stack_menu_.top().end()) {
        callable->second.function();
      } else {
        std::cout << "incorrect command " << command << std::endl;
      }
    } catch (const std::exception& ex) {
      std::cout << ex.what() << std::endl;
    }
  }

  std::stringstream ReadInputAsStringStream() {
    std::string input;
    std::cin >> std::ws;
    std::getline(std::cin, input);
    return std::stringstream(input);
  }
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_VIEW_BASEVIEW_H_
