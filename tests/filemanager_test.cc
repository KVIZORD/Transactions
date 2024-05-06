#include <filesystem>

#include "common.h"
#include "model/common/filemanager.h"
#include "model/common/student.h"
#include "model/hashtable/hash_table.h"

namespace s21 {

class FileManagerExport : public ::testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}

  HashTable<std::string, Student> storage_;
};

class FileManagerImport : public ::testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}

  HashTable<std::string, Student> storage_;
};

TEST_F(FileManagerExport, Normal) {
  storage_.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});
  storage_.Set("KEY2", Student{"NAME", "SURNAME", 12, "CITY", 5555});
  storage_.Set("KEY3", Student{"NAME", "SURNAME", 12, "CITY", 5555});
  std::string temp_file =
      (std::filesystem::temp_directory_path() / "temp.dat").string();

  FileManager::ExportToDat(storage_, temp_file);

  ASSERT_EQ(std::fstream(temp_file).is_open(), true);
  std::remove(temp_file.c_str());
  ASSERT_EQ(std::fstream(temp_file).is_open(), false);
}

TEST_F(FileManagerImport, Normal) {
  auto result =
      FileManager::ImportFromDat(storage_, kSamplesDir + "student.dat");

  ASSERT_EQ(result.first, true);
  ASSERT_EQ(result.second, 20);
  ASSERT_EQ(storage_.GetSize(), 20);
}

TEST_F(FileManagerImport, FileNotExists) {
  auto result =
      FileManager::ImportFromDat(storage_, kSamplesDir + "notexists.dat");

  ASSERT_EQ(result.first, false);
  ASSERT_EQ(result.second, 0);
  ASSERT_EQ(storage_.GetSize(), 0);
}

TEST_F(FileManagerImport, FileIncorrectStucture) {
  auto result = FileManager::ImportFromDat(
      storage_, kSamplesDir + "student_incorrect.dat");

  ASSERT_EQ(result.first, true);
  ASSERT_EQ(result.second, 0);
  ASSERT_EQ(storage_.GetSize(), 0);
}
}  // namespace s21
