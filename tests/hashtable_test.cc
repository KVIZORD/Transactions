#include "common.h"
#include "model/common/student.h"
#include "model/hashtable/hash_table.h"

namespace s21 {

TEST(HashTableCopyConstructor, Normal) {
  HashTable<std::string, Student> table;
  size_t count_elements = 100;

  for (size_t i = 0; i < count_elements; ++i) {
    table.Set("KEY" + std::to_string(i),
              Student{"NAME", "SURNAME", 12, "CITY", 5555});
  }

  HashTable<std::string, Student> table_2 = table;

  auto elems1 = table.Showall();
  auto elems2 = table_2.Showall();
  for (size_t i = 0; i < count_elements; ++i) {
    ASSERT_EQ(elems1[i], elems2[i]);
  }

  ASSERT_EQ(table.GetSize(), table_2.GetSize());
  ASSERT_EQ(table.GetLoadFactor(), table_2.GetLoadFactor());
}

TEST(HashTableCopyAssignmentConstructor, Normal) {
  HashTable<std::string, Student> table;
  HashTable<std::string, Student> table_2;
  size_t count_elements = 100;

  for (size_t i = 0; i < count_elements; ++i) {
    table.Set("KEY" + std::to_string(i),
              Student{"NAME", "SURNAME", 12, "CITY", 5555});
  }

  table_2 = table;

  auto elems1 = table.Showall();
  auto elems2 = table_2.Showall();
  for (size_t i = 0; i < count_elements; ++i) {
    ASSERT_EQ(elems1[i], elems2[i]);
  }
}

TEST(HashTableMoveAssignmentConstructor, Normal) {
  HashTable<std::string, Student> table;
  HashTable<std::string, Student> table_2;
  size_t count_elements = 100;

  for (size_t i = 0; i < count_elements; ++i) {
    table.Set("KEY" + std::to_string(i),
              Student{"NAME", "SURNAME", 12, "CITY", 5555});
  }

  for (size_t i = 0; i < count_elements; ++i) {
    table_2.Set("KEY" + std::to_string(i),
                Student{"NAME", "SURNAME", 12, "CITY", 5555});
  }

  table_2 = std::move(table);

  auto elems = table_2.Showall();
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  for (size_t i = 0; i < count_elements; ++i) {
    ASSERT_EQ(elems[i], student);
  }

  ASSERT_EQ(table.GetSize(), 0);
  ASSERT_EQ(table.Showall().size(), 0);
}

TEST(HashTableMoveConstructor, Normal) {
  HashTable<std::string, Student> table;
  size_t count_elements = 100;

  for (size_t i = 0; i < count_elements; ++i) {
    table.Set("KEY" + std::to_string(i),
              Student{"NAME", "SURNAME", 12, "CITY", 5555});
  }

  HashTable<std::string, Student> table_2 = std::move(table);

  auto elems = table_2.Showall();
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  for (size_t i = 0; i < count_elements; ++i) {
    ASSERT_EQ(elems[i], student);
  }

  ASSERT_EQ(table.GetSize(), 0);
  ASSERT_EQ(table.Showall().size(), 0);
}

TEST(HashTableSet, OneElement) {
  HashTable<std::string, Student> table;

  int status = table.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  ASSERT_EQ(status, true);
  ASSERT_EQ(table.GetSize(), 1);
}

TEST(HashTableSet, ManyElement) {
  HashTable<std::string, Student> table;
  size_t count_elements = 100;

  for (size_t i = 0; i < count_elements; ++i) {
    int status = table.Set("KEY" + std::to_string(i),
                           Student{"NAME", "SURNAME", 12, "CITY", 5555});
    ASSERT_EQ(status, true);
  }

  ASSERT_EQ(table.GetSize(), count_elements);
}

TEST(HashTableSet, ElementsWithSameKey) {
  HashTable<std::string, Student> table;

  int status_1 = table.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  int status_2 =
      table.Set("KEY", Student{"NAME2", "SURNAME2", 12, "CITY", 5555});

  ASSERT_EQ(status_1, true);
  ASSERT_EQ(status_2, false);
  ASSERT_EQ(table.GetSize(), 1);
}

TEST(HashTableGet, Normal) {
  HashTable<std::string, Student> table;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  table.Set("KEY", student);

  ASSERT_EQ(table.Get("KEY"), student);
}

TEST(HashTableGet, ThrowKeyIsNotExists) {
  HashTable<std::string, Student> table;

  ASSERT_THROW(table.Get("KEY"), std::invalid_argument);
}

TEST(HashTableExists, Exists) {
  HashTable<std::string, Student> table;
  table.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = table.Exists("KEY");

  ASSERT_EQ(status, true);
}

TEST(HashTableExists, NotExistsKey) {
  HashTable<std::string, Student> table;

  bool status = table.Exists("KEY");

  ASSERT_EQ(status, false);
}

TEST(HashTableUpdate, Success) {
  HashTable<std::string, Student> table;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  Student new_student{"NAME2", "SURNAME2", 13, "CITY2", 5556};
  table.Set("KEY", student);

  bool status = table.Update("KEY", new_student);

  ASSERT_EQ(table.Get("KEY"), new_student);
  ASSERT_EQ(status, true);
}

TEST(HashTableUpdate, FailKeyIsNotExists) {
  HashTable<std::string, Student> table;

  bool status =
      table.Update("KEY", Student{"NAME2", "SURNAME2", 13, "CITY2", 5556});

  ASSERT_EQ(status, false);
}

TEST(HashTableKeys, Normal) {
  HashTable<std::string, Student> table;
  std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

  for (auto key : keys) {
    table.Set(key, Student{"NAME", "SURNAME", 12, "CITY", 5555});
  }

  std::vector<std::string> result = table.Keys();
  std::set<std::string> result_set(result.begin(), result.end());

  ASSERT_EQ(keys.size(), result.size());
  ASSERT_EQ(result_set, keys);
}

TEST(HashTableRename, Normal) {
  HashTable<std::string, Student> table;
  table.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = table.Rename("KEY", "KEY2");

  ASSERT_EQ(table.Exists("KEY"), false);
  ASSERT_EQ(table.Exists("KEY2"), true);
  ASSERT_EQ(status, true);
}

TEST(HashTableRename, InvalidKey) {
  HashTable<std::string, Student> table;

  bool status = table.Rename("KEY", "KEY2");

  ASSERT_EQ(table.Exists("KEY"), false);
  ASSERT_EQ(table.Exists("KEY2"), false);
  ASSERT_EQ(status, false);
}

TEST(HashTableDel, Normal) {
  HashTable<std::string, Student> table;
  table.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = table.Del("KEY");

  ASSERT_EQ(status, true);
  ASSERT_EQ(table.Exists("KEY"), false);
}

TEST(HashTableDel, InvalidKey) {
  HashTable<std::string, Student> table;

  bool status = table.Del("KEY");

  ASSERT_EQ(status, false);
}

TEST(HashTableFind, OneElements) {
  HashTable<std::string, Student> table;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  table.Set("KEY", student);

  auto keys = table.Find(student);

  ASSERT_EQ(keys[0], "KEY");
}

TEST(HashTableFind, SomeElements) {
  HashTable<std::string, Student> table;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

  for (auto key : keys) {
    table.Set(key, student);
  }

  auto keys_result = table.Find(student);
  std::set<std::string> result_set(keys_result.begin(), keys_result.end());

  ASSERT_EQ(keys, result_set);
}

TEST(HashTableFind, NotExistsElement) {
  HashTable<std::string, Student> table;

  auto keys = table.Find(Student{"NAME", "SURNAME", 12, "CITY", 5555});

  ASSERT_EQ(keys.empty(), true);
}

TEST(HashTableShowAll, EmptyTable) {
  HashTable<std::string, Student> table;

  auto values = table.Showall();

  ASSERT_EQ(values.empty(), true);
}

TEST(HashTableShowAll, OneElement) {
  HashTable<std::string, Student> table;

  auto values = table.Showall();

  ASSERT_EQ(values.empty(), true);
}

TEST(HashTableShowAll, SomeElements) {
  HashTable<std::string, Student> table;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  table.Set("KEY", student);
  table.Set("KEY2", student);
  table.Set("KEY3", student);

  auto values = table.Showall();

  ASSERT_EQ(values.size(), 3);
  ASSERT_EQ(table.GetSize(), 3);
}
}  // namespace s21
