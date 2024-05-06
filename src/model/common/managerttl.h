#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <list>
#include <map>
#include <mutex>

#include "model/common/basestorage.h"

namespace s21 {

template <typename Key, typename Value>
class ManagerTTL {
 public:
  using time_type = std::chrono::steady_clock::time_point;

  struct Record {
    Key key;
    time_type death_time;

    Record(Key key, size_t ttl)
        : key(key),
          death_time(std::chrono::seconds(ttl) +
                     std::chrono::steady_clock::now()) {}
  };

  explicit ManagerTTL(BaseStorage<Key, Value>& storage) : storage_(storage) {}

  void AddRecord(const Record& record) {
    if (record.death_time <= std::chrono::steady_clock::now()) {
      return;
    }

    std::unique_lock lock(records_mtx_);
    auto it = records_.begin();
    while (it != records_.end() && it->death_time < record.death_time) {
      ++it;
    }

    records_.insert(it, record);
    ttl_info_[record.key] = record.death_time;
  }

  void DeleteExpiredRecords() {
    std::unique_lock lock(storage_mtx_);
    std::unique_lock lock2(records_mtx_);
    auto now = std::chrono::steady_clock::now();

    while (!records_.empty() && records_.begin()->death_time <= now) {
      storage_.Del(records_.begin()->key);
      ttl_info_[records_.begin()->key] = records_.begin()->death_time;
      records_.erase(records_.begin());
    }
  }

  void RenameRecord(const Key& key, const Key& new_key) {
    std::unique_lock lock_2(records_mtx_);

    for (auto it = records_.begin(); it != records_.end(); ++it) {
      if (it->key == key) {
        ttl_info_[new_key] = (it->death_time);
        ttl_info_.erase(it->key);
        it->key = new_key;
      }
    }
  }

  void DeleteRecord(const Key& key) {
    std::unique_lock lock_2(records_mtx_);

    for (auto it = records_.begin(); it != records_.end(); ++it) {
      if (it->key == key) {
        ttl_info_.erase(it->key);
        records_.erase(it);
        break;
      }
    }
  }

  void StartManagerLoop(std::chrono::seconds sleep_time) {
    running_collector_.store(true);
    while (running_collector_.load()) {
      DeleteExpiredRecords();

      std::mutex temp;
      std::unique_lock lock(temp);
      loop_condition_.wait_for(lock, sleep_time);
    }
  }

  void StopManagerLoop() {
    running_collector_.store(false);
    loop_condition_.notify_all();
  }

  size_t GetTTL(const Key& key) {
    using namespace std::chrono;

    auto elem = ttl_info_.find(key);
    if (elem == ttl_info_.end()) {
      return 0;
    }

    int time_diff =
        duration_cast<seconds>(elem->second - steady_clock::now()).count();

    return time_diff < 0 ? 0 : static_cast<size_t>(time_diff);
  }

  template <typename Func, typename... Args>
  auto ExecuteStorageOperation(Func func, Args... args) {
    DeleteExpiredRecords();
    std::unique_lock lock(storage_mtx_);
    auto result = std::invoke(func, storage_, std::forward<Args>(args)...);

    return result;
  }

 private:
  BaseStorage<Key, Value>& storage_;
  std::list<Record> records_;
  std::map<Key, time_type> ttl_info_;

  std::atomic<bool> running_collector_ = false;
  std::condition_variable loop_condition_;
  std::mutex storage_mtx_;
  std::mutex records_mtx_;
};

}  // namespace s21
#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_