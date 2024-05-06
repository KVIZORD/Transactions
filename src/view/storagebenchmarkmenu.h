#ifndef TRANSACTIONS_SOURCE_VIEW_STORAGEBENCHMARKMENU_H_
#define TRANSACTIONS_SOURCE_VIEW_STORAGEBENCHMARKMENU_H_

#include "controller/controller.h"
#include "model/bplustree/b_plus_tree.h"
#include "model/bst/self_balancing_binary_search_tree.h"
#include "model/common/storagebenchmark.h"
#include "model/hashtable/hash_table.h"
#include "view/baseview.h"

namespace s21 {
template <typename Key, typename Value>
class StorageBenchmarkMenu : BaseView {
 public:
  const std::map<std::string, MenuAction> kBenchmarkMenuCommands = {
      {"1", {[this] { HashTable(); }, "hashtable benchmark"}},
      {"2", {[this] { BPlusTree(); }, "bplusTree benchmark"}},
      {"3", {[this] { SBBST(); }, "sbbst benchmark"}},
      {"4", {[this] { PopMenu(); }, "exit"}}};

  void Start() override {
    stack_menu_.push(kBenchmarkMenuCommands);

    while (stack_menu_.size() > 0 && !std::cin.eof()) {
      DisplayMenu(stack_menu_.top());
      std::cout << "> ";
      std::string command;
      std::cin >> command;

      ExecuteCommand(command);
    }
  }

 private:
  void MeasureTimeStorage(Controller<Key, Value> &controller,
                          size_t count_elements, size_t repeats) {
    StorageBenchmark benchmark_;
    benchmark_.FillStorage(controller, count_elements);

    std::cout << "Get: ";
    std::cout << benchmark_.MeasureGet(controller, repeats) << std::endl;
    std::cout << "Showall: ";
    std::cout << benchmark_.MeasureShowAll(controller, repeats) << std::endl;
    std::cout << "Find: ";
    std::cout << benchmark_.MeasureFind(controller, repeats) << std::endl;
    std::cout << "Set: ";
    std::cout << benchmark_.MeasureSet(controller, repeats) << std::endl;
    std::cout << "Del: ";
    std::cout << benchmark_.MeasureDel(controller, repeats) << std::endl;
  }

  void HashTable() {
    s21::HashTable<Key, Value> hashtable;
    Controller controller(hashtable);

    auto input = InputCountAndRepeats();
    MeasureTimeStorage(controller, input.first, input.second);
  }

  void SBBST() {
    SelfBalancingBinarySearchTree<Key, Value> sbbst;
    Controller controller(sbbst);

    auto input = InputCountAndRepeats();
    MeasureTimeStorage(controller, input.first, input.second);
  }

  void BPlusTree() {
    s21::BPlusTree<Key, Value> bplustree;
    Controller controller(bplustree);

    auto input = InputCountAndRepeats();
    MeasureTimeStorage(controller, input.first, input.second);
  }

  std::pair<size_t, size_t> InputCountAndRepeats() {
    std::cout << "count: ";
    size_t count = parser_.ParseValue<int>(std::cin, "count");
    std::cout << "repeats: ";
    size_t repeats = parser_.ParseValue<int>(std::cin, "repeats");

    return std::make_pair(count, repeats);
  }
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_VIEW_STORAGEBENCHMARKMENU_H_
