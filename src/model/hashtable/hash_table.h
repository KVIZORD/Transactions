#ifndef TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_
#define TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_

#include <list>
#include <stdexcept>

#include "model/common/basestorage.h"

namespace s21 {

template <typename Key, typename Value,
          typename ValueEqual = std::equal_to<Value>,
          typename Hasher = std::hash<Key>>
class HashTable : public BaseStorage<Key, Value> {
 public:
  struct Node {
    Key key;
    Value value;
    int cached;
  };

  using iterator = typename std::list<Node>::iterator;
  using const_iterator = typename std::list<Node>::const_iterator;

  static constexpr size_t kDefaultSize = 8;
  static constexpr float kResizeCoeff = 0.75;
  static constexpr float kScaleCoeff = 2;

  HashTable() = default;

  ~HashTable() override {
    allocator_.deallocate(bucket_pointers_, table_size_);
  }

  HashTable(const HashTable& other) : data_(other.data_) {
    bucket_pointers_ = allocator_.allocate(other.table_size_);

    for (std::size_t i = 0; i < other.table_size_; ++i) {
      std::allocator_traits<decltype(allocator_)>::construct(
          allocator_, &bucket_pointers_[i], data_.end());
    }

    for (auto it = data_.begin(); it != data_.end(); ++it) {
      bucket_pointers_[it->cached] = it;
    }
  }

  HashTable(HashTable&& other) noexcept { swap(*this, other); }

  HashTable& operator=(const HashTable& other) {
    HashTable temp = HashTable(other);
    swap(*this, temp);
    return *this;
  }

  HashTable& operator=(HashTable&& other) noexcept {
    allocator_.deallocate(bucket_pointers_, table_size_);
    data_.clear();
    table_size_ = 0;
    bucket_pointers_ = nullptr;

    swap(*this, other);
    return *this;
  }

  bool Set(const Key& key, const Value& value) override {
    if (Exists(key)) {
      return false;
    }

    if (GetLoadFactor() >= kResizeCoeff || !bucket_pointers_) {
      Resize();
    }

    int hash = GetHash(key, table_size_);
    bucket_pointers_[hash] =
        data_.insert(bucket_pointers_[hash], Node{key, value, hash});

    return true;
  }

  Value Get(const Key& key) const override {
    auto pos = GetNodePosition(key, GetHash(key, table_size_));
    if (pos != data_.end()) {
      return pos->value;
    }

    throw std::invalid_argument("Key is not exists");
  }

  bool Exists(const Key& key) const override {
    return GetNodePosition(key, GetHash(key, table_size_)) != data_.end();
  }

  bool Del(const Key& key) override {
    auto pos = GetNodePosition(key, GetHash(key, table_size_));
    if (pos != data_.end()) {
      if (bucket_pointers_[pos->cached]->key == key) {
        bucket_pointers_[pos->cached] = pos->cached == std::next(pos)->cached
                                            ? (std::next(pos))
                                            : data_.end();
      }
      data_.erase(pos);
      return true;
    }

    return false;
  }

  bool Update(const Key& key, const Value& value) override {
    auto pos = GetNodePosition(key, GetHash(key, table_size_));
    if (pos != data_.end()) {
      pos->value = value;
      return true;
    }

    return false;
  }

  std::vector<Key> Keys() const override {
    std::vector<Key> result;

    for (auto it = data_.begin(); it != data_.end(); ++it) {
      result.push_back(it->key);
    }

    return result;
  }

  bool Rename(const Key& key, const Key& new_key) override {
    auto pos = GetNodePosition(key, GetHash(key, table_size_));

    if (pos != data_.end() && !Exists(new_key)) {
      Set(new_key, pos->value);
      Del(key);

      return true;
    }

    return false;
  }

  std::vector<Key> Find(const Value& value) const override {
    std::vector<Key> result;
    ValueEqual equal;

    for (auto it = data_.begin(); it != data_.end(); ++it) {
      if (equal(it->value, value)) {
        result.push_back(it->key);
      }
    }

    return result;
  }

  std::vector<Value> Showall() const override {
    std::vector<Value> result;

    for (auto it = data_.begin(); it != data_.end(); ++it) {
      result.push_back(it->value);
    }

    return result;
  }

  std::size_t GetSize() const { return data_.size(); }

  float GetLoadFactor() const {
    return table_size_ == 0 ? 0 : data_.size() / table_size_;
  }

 private:
  size_t GetHash(const Key& value, size_t size) const {
    return size == 1 ? 0 : hasher_(value) % (size - 1);
  }

  const_iterator GetNodePosition(const Key& key, int hash) const {
    if (bucket_pointers_ == nullptr) {
      return data_.end();
    }

    for (auto it = bucket_pointers_[hash];
         it != data_.end() && it->cached == hash; ++it) {
      if (it->key == key) {
        return it;
      }
    }

    return data_.end();
  }

  iterator GetNodePosition(const Key& key, int hash) {
    if (bucket_pointers_ == nullptr) {
      return data_.end();
    }

    for (auto it = bucket_pointers_[hash];
         it != data_.end() && it->cached == hash; ++it) {
      if (it->key == key) {
        return it;
      }
    }

    return data_.end();
  }

  void Resize() {
    size_t new_table_size =
        table_size_ == 0 ? kDefaultSize : table_size_ * kScaleCoeff;
    iterator* new_bucket_pointers = allocator_.allocate(new_table_size);
    std::list<Node> temp;
    std::swap(temp, data_);

    for (std::size_t i = 0; i < new_table_size; ++i) {
      std::allocator_traits<std::allocator<iterator>>::construct(
          allocator_, &new_bucket_pointers[i], data_.end());
    }

    for (auto it = temp.begin(); it != temp.end(); ++it) {
      int new_hash = GetHash(it->key, new_table_size);
      new_bucket_pointers[new_hash] = data_.insert(
          new_bucket_pointers[new_hash], Node{it->key, it->value, new_hash});
    }

    allocator_.deallocate(bucket_pointers_, table_size_);
    bucket_pointers_ = std::move(new_bucket_pointers);
    table_size_ = new_table_size;
  }

  friend void swap(HashTable& first, HashTable& second) noexcept {
    using std::swap;

    swap(first.table_size_, second.table_size_);
    swap(first.data_, second.data_);
    swap(first.bucket_pointers_, second.bucket_pointers_);
    swap(first.hasher_, second.hasher_);
    swap(first.allocator_, second.allocator_);
  }

  size_t table_size_ = 0;
  std::list<Node> data_{};
  iterator* bucket_pointers_ = nullptr;
  Hasher hasher_{};
  std::allocator<iterator> allocator_{};
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_