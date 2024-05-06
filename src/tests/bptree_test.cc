#include "common.h"
#include "model/bplustree/b_plus_tree.h"
#include "model/common/student.h"

namespace s21 {

TEST(BPlusTreeCopyConstructor, NotEmptyTree) {
  BPlusTree<std::string, Student> tree;
  int status = tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});
  ASSERT_EQ(status, true);

  BPlusTree<std::string, Student> tree_copy(tree);

  ASSERT_EQ(tree.Exists("KEY"), true);
  ASSERT_EQ(tree_copy.Exists("KEY"), true);
  ASSERT_EQ(tree.Keys(), tree_copy.Keys());


  tree_copy.Update("KEY", Student{"Test", "Testov", 9, "city", 1111});

  ASSERT_EQ(tree.Get("KEY").name, "NAME");
  ASSERT_EQ(tree.Get("KEY").surname, "SURNAME");
  ASSERT_EQ(tree.Get("KEY").birth_year, 12);
  ASSERT_EQ(tree.Get("KEY").city, "CITY");
  ASSERT_EQ(tree.Get("KEY").coins, 5555);

  ASSERT_EQ(tree_copy.Get("KEY").name, "Test");
  ASSERT_EQ(tree_copy.Get("KEY").surname, "Testov");
  ASSERT_EQ(tree_copy.Get("KEY").birth_year, 9);
  ASSERT_EQ(tree_copy.Get("KEY").city, "city");
  ASSERT_EQ(tree_copy.Get("KEY").coins, 1111);

}

TEST(BPlusTreeCopyConstructor, EmptyTree) {
  BPlusTree<std::string, Student> tree;
  BPlusTree<std::string, Student> tree_copy(tree);

  ASSERT_EQ(tree.Keys().empty(), true);
  ASSERT_EQ(tree_copy.Keys().empty(), true);
}

TEST(BPlusTreeMoveConstructor, NotEmptyTree) {
  BPlusTree<std::string, Student> tree;
  int status = tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});
  ASSERT_EQ(status, true);

  BPlusTree<std::string, Student> tree_move(std::move(tree));

  ASSERT_EQ(tree.Exists("KEY"), false);
  ASSERT_EQ(tree_move.Exists("KEY"), true);
}

TEST(BPlusTreeMoveConstructor, EmptyTree) {
  BPlusTree<std::string, Student> tree;
  BPlusTree<std::string, Student> tree_move(std::move(tree));

  ASSERT_EQ(tree.Keys().empty(), true);
  ASSERT_EQ(tree_move.Keys().empty(), true);
}

TEST(BPlusTreeSet, OneElement) {
  BPlusTree<std::string, Student> tree;

  int status = tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  ASSERT_EQ(status, true);
}

TEST(BPlusTreeSet, ManyElement) {
  BPlusTree<std::string, Student> tree;
  size_t count_elements = 100;

  for (size_t i = 0; i < count_elements; ++i) {
    int status = tree.Set("KEY" + std::to_string(i),
                           Student{"NAME", "SURNAME", 12, "CITY", 5555});
    ASSERT_EQ(status, true);
  }
}

TEST(BPlusTreeSet, ElementsWithSameKey) {
  BPlusTree<std::string, Student> tree;

  int status_1 = tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  int status_2 =
      tree.Set("KEY", Student{"NAME2", "SURNAME2", 12, "CITY", 5555});

  ASSERT_EQ(status_1, true);
  ASSERT_EQ(status_2, false);
}

TEST(BPlusTreeGet, ThrowKeyIsNotExists) {
  BPlusTree<std::string, Student> tree;

  ASSERT_THROW(tree.Get("KEY"), std::invalid_argument);
}

TEST(BPlusTreeExists, Exists) {
  BPlusTree<std::string, Student> tree;
  tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = tree.Exists("KEY");

  ASSERT_EQ(status, true);
}

TEST(BPlusTreeExists, NotExistsKey) {
  BPlusTree<std::string, Student> tree;

  bool status = tree.Exists("KEY");

  ASSERT_EQ(status, false);
}

TEST(BPlusTreeUpdate, Success) {
  BPlusTree<std::string, Student> tree;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  Student new_student{"NAME2", "SURNAME2", 13, "CITY2", 5556};
  tree.Set("KEY", student);

  bool status = tree.Update("KEY", new_student);

  ASSERT_EQ(tree.Get("KEY"), new_student);
  ASSERT_EQ(status, true);
}

TEST(BPlusTreeUpdate, FailKeyIsNotExists) {
  BPlusTree<std::string, Student> tree;

  bool status =
      tree.Update("KEY", Student{"NAME2", "SURNAME2", 13, "CITY2", 5556});

  ASSERT_EQ(status, false);
}

TEST(BPlusTreeKeys, Normal) {
  BPlusTree<std::string, Student> tree;
  std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

  for (auto key : keys) {
    tree.Set(key, Student{"NAME", "SURNAME", 12, "CITY", 5555});
  }

  std::vector<std::string> result = tree.Keys();
  std::set<std::string> result_set(result.begin(), result.end());

  ASSERT_EQ(keys.size(), result.size());
  ASSERT_EQ(result_set, keys);
}

TEST(BPlusTreeRename, Normal) {
  BPlusTree<std::string, Student> tree;
  tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = tree.Rename("KEY", "KEY2");

  ASSERT_EQ(tree.Exists("KEY"), false);
  ASSERT_EQ(tree.Exists("KEY2"), true);
  ASSERT_EQ(status, true);
}

TEST(BPlusTreeRename, InvalidKey) {
  BPlusTree<std::string, Student> tree;

  bool status = tree.Rename("KEY", "KEY2");

  ASSERT_EQ(tree.Exists("KEY"), false);
  ASSERT_EQ(tree.Exists("KEY2"), false);
  ASSERT_EQ(status, false);
}

TEST(BPlusTreeDel, Normal) {
  BPlusTree<std::string, Student> tree;
  tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = tree.Del("KEY");

  ASSERT_EQ(status, true);
  ASSERT_EQ(tree.Exists("KEY"), false);
}

TEST(BPlusTreeDel, InvalidKey) {
  BPlusTree<std::string, Student> tree;

  bool status = tree.Del("KEY");

  ASSERT_EQ(status, false);
}

TEST(BPlusTreeFind, OneElements) {
  BPlusTree<std::string, Student> tree;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  tree.Set("KEY", student);

  auto keys = tree.Find(student);

  ASSERT_EQ(keys[0], "KEY");
}

TEST(BPlusTreeFind, SomeElements) {
  BPlusTree<std::string, Student> tree;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

  for (auto key : keys) {
    tree.Set(key, student);
  }

  auto keys_result = tree.Find(student);
  std::set<std::string> result_set(keys_result.begin(), keys_result.end());

  ASSERT_EQ(keys, result_set);
}

TEST(BPlusTreeFind, NotExistsElement) {
  BPlusTree<std::string, Student> tree;

  auto keys = tree.Find(Student{"NAME", "SURNAME", 12, "CITY", 5555});

  ASSERT_EQ(keys.empty(), true);
}

TEST(BPlusTreeShowAll, Emptytree) {
  BPlusTree<std::string, Student> tree;

  auto values = tree.Showall();

  ASSERT_EQ(values.empty(), true);
}

TEST(BPlusTreeShowAll, OneElement) {
  BPlusTree<std::string, Student> tree;

  auto values = tree.Showall();

  ASSERT_EQ(values.empty(), true);
}

TEST(BPlusTreeShowAll, SomeElements) {
  BPlusTree<std::string, Student> tree;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  tree.Set("KEY", student);
  tree.Set("KEY2", student);
  tree.Set("KEY3", student);

  auto values = tree.Showall();

  ASSERT_EQ(values.size(), 3);
}
}  // namespace s21
