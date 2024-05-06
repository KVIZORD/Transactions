#ifndef TRANSACTIONS_SOURCE_MODEL_BPLUSTREE_B_PLUS_TREE_H_
#define TRANSACTIONS_SOURCE_MODEL_BPLUSTREE_B_PLUS_TREE_H_

#include <fstream>
#include <stdexcept>

#include "model/bplustree/tree.h"
#include "model/common/basestorage.h"

namespace s21 {

template <typename Key, typename Value,
          typename ValueEqual = std::equal_to<Value>>
class BPlusTree : public BaseStorage<Key, Value> {
 public:
  template <typename T>
  struct NewValue {
    int ttl;
    T value;
  };
  using new_value = NewValue<Value>;

  ~BPlusTree() = default;

  BPlusTree() {}

  BPlusTree(const BPlusTree& other) : tree_(other.tree_) {}

  BPlusTree(BPlusTree&& other) noexcept : tree_(std::move(other.tree_)) {}

  bool Set(const Key& key, const Value& value) {
    return tree_.Insert(key, new_value{0, value});
  };

  Value Get(const Key& key) const {
    const new_value& value = tree_.Search(key);
    return value.value;
  };

  bool Exists(const Key& key) const { return tree_.Exists(key); };

  bool Del(const Key& key) { return tree_.Remove(key); };

  bool Update(const Key& key, const Value& value) {
    try {
      const new_value& old = tree_.Search(key);
      return tree_.Update(key, new_value{old.ttl, value});
    } catch (const std::invalid_argument& err) {
      return false;
    }
  };

  std::vector<Key> Keys() const {
    std::vector<Key> keys;
    for (auto it = tree_.Begin(); it != tree_.End(); ++it) {
      keys.push_back((*it).first);
    }
    return keys;
  };

  bool Rename(const Key& key, const Key& new_key) {
    if (tree_.Exists(new_key) || !tree_.Exists(key)) {
      return false;
    }
    const new_value value = tree_.Search(key);
    bool res = tree_.Remove(key);
    if (!res) {
      return false;
    }
    return tree_.Insert(new_key, value);
  };

  int Ttl(const Key& key) const {
    const new_value& value = tree_.Search(key);
    return value.ttl;
  };

  std::vector<Key> Find(const Value& value) const {
    std::vector<Key> result;
    ValueEqual equal;

    for (auto it = tree_.Begin(); it != tree_.End(); ++it) {
      if (equal((*it).second.value, value)) {
        result.push_back((*it).first);
      }
    }

    return result;
  };

  std::vector<Value> Showall() const {
    std::vector<Value> values;
    for (auto it = tree_.Begin(); it != tree_.End(); ++it) {
      values.push_back((*it).second.value);
    }
    return values;
  };

  std::pair<bool, int> Upload(const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
      return std::pair<bool, int>(false, 0);
    }

    Key key;
    Value value;
    int counter = 0;
    while (file >> key && file >> value) {
      if (Set(std::move(key), std::move(value))) {
        ++counter;
      }
    }

    return std::pair<bool, int>(true, counter);
  };

  std::pair<bool, int> Export(const std::string& path) const {
    std::ofstream file(path);

    if (!file.is_open()) {
      return std::pair<bool, int>(false, 0);
    }

    int counter = 0;
    for (auto it = tree_.Begin(); it != tree_.End(); ++it) {
      file << (*it).first << " " << (*it).second.value << std::endl;
      ++counter;
    }

    return std::pair<bool, int>(true, counter);
  };

  void Print() { tree_.printTree(); }

 private:
  Tree<Key, new_value> tree_;
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_BPLUSTREE_B_PLUS_TREE_H_