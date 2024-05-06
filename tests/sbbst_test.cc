#include "common.h"
#include "model/bst/self_balancing_binary_search_tree.h"
#include "model/common/student.h"

namespace s21 {
TEST(BstSet, OneElement) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  int status = bst.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  ASSERT_EQ(status, true);
}

TEST(BstSet, ManyElement) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  size_t count_elements = 100;

  for (size_t i = 0; i < count_elements; ++i) {
    int status = bst.Set("KEY" + std::to_string(i),
                         Student{"NAME", "SURNAME", 12, "CITY", 5555});
    ASSERT_EQ(status, true);
  }
}

TEST(BstSet, ElementsWithSameKey) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  int status_1 = bst.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  int status_2 = bst.Set("KEY", Student{"NAME2", "SURNAME2", 12, "CITY", 5555});

  ASSERT_EQ(status_1, true);
  ASSERT_EQ(status_2, false);
}

TEST(BstGet, Normal) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  bst.Set("KEY", student);

  ASSERT_EQ(bst.Get("KEY"), student);
}

TEST(BstGet, ThrowKeyIsNotExists) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  ASSERT_THROW(bst.Get("KEY"), std::invalid_argument);
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  bst.Set("A", student);
  bst.Set("B", student);
  bst.Set("C", student);
  bst.Set("D", student);
  ASSERT_THROW(bst.Get("B1"), std::invalid_argument);
}

TEST(BstExists, Exists) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  bst.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = bst.Exists("KEY");

  ASSERT_EQ(status, true);
}

TEST(BstExists, NotExistsKey) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  bool status = bst.Exists("KEY");

  ASSERT_EQ(status, false);
}

TEST(BstUpdate, Success) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  Student new_student{"NAME2", "SURNAME2", 13, "CITY2", 5556};
  bst.Set("KEY", student);

  bool status = bst.Update("KEY", new_student);

  ASSERT_EQ(bst.Get("KEY"), new_student);
  ASSERT_EQ(status, true);
}

TEST(BstUpdate, FailKeyIsNotExists) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  bool status =
      bst.Update("KEY", Student{"NAME2", "SURNAME2", 13, "CITY2", 5556});

  ASSERT_EQ(status, false);
}

TEST(BstKeys, Normal) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

  for (auto key : keys) {
    bst.Set(key, Student{"NAME", "SURNAME", 12, "CITY", 5555});
  }

  std::vector<std::string> result = bst.Keys();
  std::set<std::string> result_set(result.begin(), result.end());

  ASSERT_EQ(keys.size(), result.size());
  ASSERT_EQ(result_set, keys);
}

TEST(BstRename, Normal) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  bst.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = bst.Rename("KEY", "KEY2");

  ASSERT_EQ(bst.Exists("KEY"), false);
  ASSERT_EQ(bst.Exists("KEY2"), true);
  ASSERT_EQ(status, true);
}

TEST(BstRename, InvalidKey) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  bool status = bst.Rename("KEY", "KEY2");

  ASSERT_EQ(bst.Exists("KEY"), false);
  ASSERT_EQ(bst.Exists("KEY2"), false);
  ASSERT_EQ(status, false);
}

TEST(BstDel, Normal) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  bst.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = bst.Del("KEY");

  ASSERT_EQ(status, true);
  ASSERT_EQ(bst.Exists("KEY"), false);
}

TEST(BstDel, InvalidKey) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  bool status = bst.Del("KEY");

  ASSERT_EQ(status, false);
}

TEST(BstDel, Rebalance) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  bst.Set("A", student);
  bst.Set("B", student);
  bst.Set("C", student);
  bst.Set("D", student);
  bst.Set("E", student);
  bst.Set("F", student);
  bst.Set("G", student);
  bst.Set("H", student);
  bst.Set("I", student);
  bst.Set("K", student);
  bst.Set("J", student);

  bool status1 = bst.Del("J");
  bool status2 = bst.Del("F");
  bool status3 = bst.Del("H");
  bool status4 = bst.Del("E");
  bool status5 = bst.Del("B");
  bool status6 = bst.Del("G");
  bool status7 = bst.Del("C");
  bool status8 = bst.Del("A");
  bool status9 = bst.Del("D");
  bool status10 = bst.Del("I");
  bool status11 = bst.Del("K");

  ASSERT_EQ(status1, true);
  ASSERT_EQ(status2, true);
  ASSERT_EQ(status3, true);
  ASSERT_EQ(status4, true);
  ASSERT_EQ(status5, true);
  ASSERT_EQ(status6, true);
  ASSERT_EQ(status7, true);
  ASSERT_EQ(status8, true);
  ASSERT_EQ(status9, true);
  ASSERT_EQ(status10, true);
  ASSERT_EQ(status11, true);
}

TEST(BstFind, OneElements) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  bst.Set("KEY", student);

  auto keys = bst.Find(student);

  ASSERT_EQ(keys[0], "KEY");
}

TEST(BstFind, SomeElements) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

  for (auto key : keys) {
    bst.Set(key, student);
  }

  auto keys_result = bst.Find(student);
  std::set<std::string> result_set(keys_result.begin(), keys_result.end());

  ASSERT_EQ(keys, result_set);
}

TEST(BstFind, NotExistsElement) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  auto keys = bst.Find(Student{"NAME", "SURNAME", 12, "CITY", 5555});

  ASSERT_EQ(keys.empty(), true);
}

TEST(BstShowAll, Emptybst) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  auto values = bst.Showall();

  ASSERT_EQ(values.empty(), true);
}

TEST(BstShowAll, OneElement) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  auto values = bst.Showall();

  ASSERT_EQ(values.empty(), true);
}

TEST(BstShowAll, SomeElements) {
  SelfBalancingBinarySearchTree<std::string, Student> bst;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  bst.Set("KEY", student);
  bst.Set("KEY2", student);
  bst.Set("KEY3", student);

  auto values = bst.Showall();

  ASSERT_EQ(values.size(), 3);
}
}  // namespace s21
