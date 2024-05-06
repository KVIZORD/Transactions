#ifndef TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_
#define TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_

#include "model/common/basestorage.h"
#include "model/common/filemanager.h"
#include "model/common/managerttl.h"

namespace s21 {

template <typename Key, typename Value>
class Controller {
 public:
  explicit Controller(BaseStorage<Key, Value>& model)
      : manager_(model), model_(model) {}

  bool Set(Key key, Value value, size_t ttl = 0) {
    manager_.AddRecord({key, ttl});
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Set, key,
                                            value);
  }

  Value Get(Key key) {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Get, key);
  }

  bool Exists(Key key) {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Exists,
                                            key);
  }

  bool Del(Key key) {
    bool status =
        manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Del, key);
    manager_.DeleteRecord(key);
    return status;
  }

  bool Update(Key key, Value value) {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Update,
                                            key, value);
  }

  std::vector<Key> Keys() {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Keys);
  }

  bool Rename(Key key, Key new_key) {
    manager_.RenameRecord(key, new_key);
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Rename,
                                            key, new_key);
  }

  size_t TTL(Key key) { return manager_.GetTTL(key); }

  std::vector<Key> Find(Value value) {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Find,
                                            value);
  }

  std::vector<Value> Showall() {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Showall);
  }

  std::pair<bool, size_t> Upload(const std::string& path) {
    return s21::FileManager::ImportFromDat(model_, path);
  }

  std::pair<bool, size_t> Export(const std::string& path) const {
    return s21::FileManager::ExportToDat(model_, path);
  }

  void StartManagerLoop(std::chrono::seconds sleep_time) {
    manager_.StartManagerLoop(sleep_time);
  }

  void StopManagerLoop() { manager_.StopManagerLoop(); }

 private:
  ManagerTTL<Key, Value> manager_;
  BaseStorage<Key, Value>& model_;
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_
