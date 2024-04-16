#ifndef TRANSACTIONS_SOURCE_VIEW_STORAGEMENU_H_
#define TRANSACTIONS_SOURCE_VIEW_STORAGEMENU_H_

#include <view/baseview.h>

#include <functional>
#include <map>
#include <sstream>
#include <stack>
#include <string>

#include "controller/controller.h"
#include "model/parser/parser.h"

namespace s21 {
template <typename Key, typename Value>
class StorageMenu : BaseView {
 public:
  const std::map<std::string, MenuAction> kStorageCommands = {
      {"set", {[this] { Set(); }, "<key> <struct> [ex <time>]"}},
      {"get", {[this] { Get(); }, "<key>"}},
      {"exists", {[this] { Exists(); }, "<key>"}},
      {"del", {[this] { Del(); }, "<key>"}},
      {"update", {[this] { Update(); }, "<key> <struct>"}},
      {"keys", {[this] { Keys(); }, ""}},
      {"rename", {[this] { Rename(); }, "<key1> <key2>"}},
      {"ttl", {[this] { Ttl(); }, "<key>"}},
      {"find", {[this] { Find(); }, "<struct>"}},
      {"showall", {[this] { Showall(); }, ""}},
      {"upload", {[this] { Upload(); }, "<path>"}},
      {"export", {[this] { Export(); }, "<path>"}},
      {"help", {[this] { DisplayMenu(kStorageCommands); }, ""}},
      {"exit", {[this] { PopMenu(); }, ""}}};

  explicit StorageMenu(const Controller<Key, Value>& controller)
      : controller_(controller) {}

  void Start() override {
    PushMenu(kStorageCommands);
    DisplayMenu(kStorageCommands);

    while (stack_menu_.size() > 0) {
      std::string command;
      std::cout << "> ";
      std::cin >> command;

      ExecuteCommand(command);
    }
  }

 private:
  Controller<Key, Value> controller_;

  void Set() {
    std::stringstream user_input = ReadInputAsStringStream();
    Key key = parser_.ParseValue<Key>(user_input, "key");
    Value value = parser_.ParseValue<Value>(user_input, "value");

    bool status = false;
    auto optional_arg = parser_.ParseOptionalArgument<int>(user_input, "ex");
    if (optional_arg.first.empty()) {
      status = controller_.Set(key, value);
    } else {
      status = controller_.Set(key, value, optional_arg.second);
    }

    std::cout << StatusToStr(status) << std::endl;
  }

  void Get() {
    std::stringstream user_input = ReadInputAsStringStream();
    Key key = parser_.ParseValue<Key>(user_input, "key");
    try {
      Value value = controller_.Get(key);
      std::cout << value << std::endl;
    } catch (std::invalid_argument& ex) {
      std::cout << "(null)" << std::endl;
    }
  }

  void Exists() {
    std::stringstream user_input = ReadInputAsStringStream();
    Key key = parser_.ParseValue<Key>(user_input, "key");
    std::cout << std::boolalpha << controller_.Exists(key) << std::endl;
  }

  void Del() {
    std::stringstream user_input = ReadInputAsStringStream();
    Key key = parser_.ParseValue<Key>(user_input, "key");
    std::cout << std::boolalpha << StatusToStr(controller_.Del(key))
              << std::endl;
  }

  void Update() {
    std::stringstream user_input = ReadInputAsStringStream();
    Key key = parser_.ParseValue<Key>(user_input, "key");
    Value value = parser_.ParseValue<Value>(user_input, "value");

    std::cout << StatusToStr(controller_.Update(key, value)) << std::endl;
  }

  void Keys() {
    size_t counter = 1;
    for (auto& key : controller_.Keys()) {
      std::cout << counter << ") " << key << std::endl;
      ++counter;
    }
  }

  void Rename() {
    std::stringstream user_input = ReadInputAsStringStream();
    Key key_1 = parser_.ParseValue<Key>(user_input, "key1");
    Key key_2 = parser_.ParseValue<Key>(user_input, "key2");
    std::cout << controller_.Rename(key_1, key_2) << std::endl;
  }

  void Ttl() {
    std::stringstream user_input = ReadInputAsStringStream();
    Key key = parser_.ParseValue<Key>(user_input, "key");
    try {
      std::cout << controller_.Ttl(key) << std::endl;
    } catch (std::invalid_argument& ex) {
      std::cout << "(null)" << std::endl;
    }
  }

  void Find() {
    std::stringstream user_input = ReadInputAsStringStream();
    Value value = parser_.ParseValue<Value>(user_input, "value");

    size_t counter = 1;
    for (auto& key : controller_.Find(value)) {
      std::cout << counter << ") " << key << std::endl;
      ++counter;
    }
  }

  void Showall() {
    size_t counter = 1;
    for (auto& value : controller_.Showall()) {
      std::cout << counter << ") " << value << std::endl;
      ++counter;
    }
  }

  void Upload() {
    std::stringstream user_input = ReadInputAsStringStream();
    std::string path = parser_.ParseValue<std::string>(user_input, "path");
    auto res = controller_.Upload(path);
    std::cout << StatusToStr(res.first) << " " << res.second << std::endl;
  }

  void Export() {
    std::stringstream user_input = ReadInputAsStringStream();
    std::string path = parser_.ParseValue<std::string>(user_input, "path");
    auto res = controller_.Export(path);
    std::cout << StatusToStr(res.first) << " " << res.second << std::endl;
  }

  static std::string StatusToStr(bool status) {
    return status ? "OK" : "(null)";
  }
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_VIEW_STORAGEMENU_H_