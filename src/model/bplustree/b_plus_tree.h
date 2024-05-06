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
  ~BPlusTree() = default;

  BPlusTree() {}

  BPlusTree(const BPlusTree& other) : tree_(other.tree_) {}

  BPlusTree(BPlusTree&& other) noexcept : tree_(std::move(other.tree_)) {}

  bool Set(const Key& key, const Value& value) {
    return tree_.Insert(key, value);
  };

  Value Get(const Key& key) const {
    const Value& value = tree_.Search(key);
    return value;
  };

  bool Exists(const Key& key) const { return tree_.Exists(key); };

  bool Del(const Key& key) { return tree_.Remove(key); };

  bool Update(const Key& key, const Value& value) {
    try {
      return tree_.Update(key, value);
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
    const Value value = tree_.Search(key);
    bool res = tree_.Remove(key);
    if (!res) {
      return false;
    }
    return tree_.Insert(new_key, value);
  };

  std::vector<Key> Find(const Value& value) const {
    std::vector<Key> result;
    ValueEqual equal;

    for (auto it = tree_.Begin(); it != tree_.End(); ++it) {
      if (equal((*it).second, value)) {
        result.push_back((*it).first);
      }
    }

    return result;
  };

  std::vector<Value> Showall() const {
    std::vector<Value> values;
    for (auto it = tree_.Begin(); it != tree_.End(); ++it) {
      values.push_back((*it).second);
    }
    return values;
  };

 private:
  Tree<Key, Value> tree_;
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_BPLUSTREE_B_PLUS_TREE_H_