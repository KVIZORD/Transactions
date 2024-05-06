#include "model/bplustree/b_plus_tree.h"
#include "model/bst/self_balancing_binary_search_tree.h"
#include "model/common/student.h"
#include "model/hashtable/hash_table.h"
#include "view/mainmenu.h"

int main() {
  s21::HashTable<std::string, s21::Student, s21::StudentComparator> hashtable;
  s21::SelfBalancingBinarySearchTree<std::string, s21::Student,
                                     s21::StudentComparator>
      sbbst;
  s21::BPlusTree<std::string, s21::Student, s21::StudentComparator> bplustree;

  s21::Controller controller_1(hashtable);
  s21::Controller controller_2(sbbst);
  s21::Controller controller_3(bplustree);

  s21::MainMenu mainmenu(controller_1, controller_2, controller_3);

  mainmenu.Start();

  return 0;
}
