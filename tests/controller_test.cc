#include <chrono>
#include <thread>

#include "common.h"
#include "controller/controller.h"
#include "model/common/student.h"
#include "model/hashtable/hash_table.h"

namespace s21 {

class ControllerFixture : public ::testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}

  HashTable<std::string, Student> ht_;
  Controller<std::string, Student> controller_{ht_};
  std::string key_ = "key";
  Student value_{"name", "surname", 2020, "nsk", 100};
};

TEST_F(ControllerFixture, TTLNormal) {
  controller_.Set(key_, value_, 5);
  size_t ttl = controller_.TTL(key_);

  ASSERT_EQ(ttl, 4);
}

TEST_F(ControllerFixture, TTLExpired) {
  controller_.Set(key_, value_, 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(1001));

  ASSERT_EQ(controller_.TTL(key_), 0);
}

TEST_F(ControllerFixture, SetNegativeTTL) {
  controller_.Set(key_, value_, -5);
  size_t ttl = controller_.TTL(key_);

  ASSERT_EQ(ttl, 0);
}

TEST_F(ControllerFixture, GetExpiredTTL) {
  controller_.Set(key_, value_, 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(1001));

  ASSERT_THROW(controller_.Get(key_), std::invalid_argument);
}

TEST_F(ControllerFixture, ExistsExpiredTTL) {
  controller_.Set(key_, value_, 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(1001));

  ASSERT_EQ(controller_.Exists(key_), false);
}

TEST_F(ControllerFixture, DelExpiredTTL) {
  controller_.Set(key_, value_, 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(1001));

  ASSERT_EQ(controller_.Del(key_), false);
}

}  // namespace s21