#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_BASESTORAGE_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_BASESTORAGE_H_

#include <vector>

namespace s21 {
template <typename Key, typename Value>
class BaseStorage {
 public:
  virtual ~BaseStorage() = default;
  virtual bool Set(const Key& key, const Value& value) = 0;
  virtual Value Get(const Key& key) const = 0;
  virtual bool Exists(const Key& key) const = 0;
  virtual bool Del(const Key& key) = 0;
  virtual bool Update(const Key& key, const Value& value) = 0;
  virtual std::vector<Key> Keys() const = 0;
  virtual bool Rename(const Key& key, const Key& new_key) = 0;
  virtual std::vector<Key> Find(const Value& value) const = 0;
  virtual std::vector<Value> Showall() const = 0;
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_BASESTORAGE_H_