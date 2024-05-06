#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_FILEMANAGER_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_FILEMANAGER_H_

#include <fstream>

#include "model/common/basestorage.h"

namespace s21 {
class FileManager {
 public:
  template <typename Key, typename Value>
  static std::pair<bool, size_t> ExportToDat(
      const BaseStorage<Key, Value>& storage, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
      return std::pair(false, 0);
    }

    std::vector<Key> keys = storage.Keys();
    std::vector<Value> values = storage.Showall();
    size_t counter = 0;
    for (auto it_key = keys.begin(), it_value = values.begin();
         it_key != keys.end() && it_value != values.end();
         ++it_key, ++it_value) {
      file << *it_key << " " << *it_value << '\n';
      ++counter;
    }

    return std::pair(true, counter);
  }

  template <typename Key, typename Value>
  static std::pair<bool, size_t> ImportFromDat(BaseStorage<Key, Value>& storage,
                                               const std::string& filename) {
    std::ifstream file(filename);
    Key key;
    Value value;

    if (!file.is_open()) {
      return std::pair(false, 0);
    }

    size_t counter = 0;
    while (file >> key && file >> value) {
      if (storage.Set(key, value)) {
        ++counter;
      }
    }

    return std::pair(true, counter);
  }
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_FILEMANAGER_H_