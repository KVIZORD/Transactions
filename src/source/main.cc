#include <model/bst/self_balancing_binary_search_tree.h>
#include <model/common/data.h>
#include <model/hashtable/hash_table.h>
#include <view/mainmenu.h>
#include <view/storagemenu.h>

#include <iostream>
#include <string>

int main() {
  s21::HashTable<std::string, s21::Student, s21::StudentComparator> hashtable;
  s21::SelfBalancingBinarySearchTree sbbst;

  s21::Controller controller_1(std::move(hashtable));
  s21::Controller controller_2(std::move(sbbst));

  s21::MainMenu mainmenu(controller_1, controller_2, controller_1);

  mainmenu.Start();

  return 0;
}
