#ifndef TRANSACTIONS_SOURCE_MODEL_BST_SELF_BALANCING_BINARY_SEARCH_TREE_H_
#define TRANSACTIONS_SOURCE_MODEL_BST_SELF_BALANCING_BINARY_SEARCH_TREE_H_

#include "../common/basestorage.h"

namespace s21 {

enum Color {
  Black,
  Red,
};

template <typename Key, typename Value>
class BSTNode {
 public:
  BSTNode* parent = nullptr;
  BSTNode* link[2] = {nullptr, nullptr};
  std::pair<Key, Value> data;
  bool color = Color::Black;

  BSTNode() = default;
  BSTNode(const Key& key, const Value& value)
      : data(std::make_pair(key, value)), color(Color::Red){};

  explicit BSTNode(const BSTNode& other)
      : data(other.data), color(other.color){};

  BSTNode& operator=(const BSTNode& other) {
    data = other.data;
    color = other.color;
    return *this;
  }
};

template <typename Key, typename Value,
          typename ValueEqual = std::equal_to<Value>>
class SelfBalancingBinarySearchTree : public BaseStorage<Key, Value> {
 public:
  class BSTIterator;
  using Node = BSTNode<Key, Value>;
  using Pointer = Node*;
  using Iterator = BSTIterator;
  using ValueType = std::pair<Key, Value>;
  Pointer leaf_;

  SelfBalancingBinarySearchTree();

  explicit SelfBalancingBinarySearchTree(
      const SelfBalancingBinarySearchTree& other);
  SelfBalancingBinarySearchTree& operator=(
      const SelfBalancingBinarySearchTree& other);
  ~SelfBalancingBinarySearchTree();

  class BSTIterator {
    friend class SelfBalancingBinarySearchTree;

   public:
    BSTIterator(Pointer node, Pointer leaf) : current_(node), leaf_(leaf) {}
    BSTIterator() : current_(nullptr), leaf_(nullptr) {}
    BSTIterator(const BSTIterator& obj) {
      current_ = obj.current_;
      leaf_ = obj.leaf_;
    }

    BSTIterator& operator=(const BSTIterator& obj) noexcept {
      current_ = obj.current_;
      leaf_ = obj.leaf_;
      return *this;
    }

    BSTIterator& operator++() noexcept {
      if (current_->link[1] != leaf_) {
        current_ = current_->link[1];
        while (current_->link[0] != leaf_) {
          current_ = current_->link[0];
        }
      } else {
        Pointer tmp = current_->parent;
        while (tmp && tmp->data.first < current_->data.first) {
          current_ = tmp;
          tmp = tmp->parent;
        }
        current_ = tmp;
      }
      return *this;
    }

    bool operator!=(const BSTIterator& other) const noexcept {
      return (current_ != other.current_);
    }

    bool IsValid() const { return current_ != nullptr; }

    ValueType* operator->() const noexcept { return &(current_->data); }

   private:
    Pointer current_;
    Pointer leaf_;
  };

  bool Set(const Key& key, const Value& value) override;
  Value Get(const Key& key) const override;
  bool Exists(const Key& key) const override;
  bool Del(const Key& key) override;
  bool Update(const Key& key, const Value& value) override;
  std::vector<Key> Keys() const override;
  bool Rename(const Key& key, const Key& new_key) override;
  std::vector<Key> Find(const Value& value) const override;
  std::vector<Value> Showall() const override;

 private:
  Pointer root_;
  void Swap(Pointer a, Pointer b);
  void LeftRotation(Pointer node);
  void RightRotation(Pointer node);
  void BalanceTree(Pointer node);
  Iterator Begin() const;
  Iterator End() const;
  void Clear();
  int CountChildren(Pointer node) const;
  Pointer GetChild(Pointer node) const;
  Pointer GetParent(Pointer node) const;
  Pointer Search(const Key& key) const;
  bool RemoveNode(Pointer node);
  void RemoveNodeWithoutChildren(Pointer node);
  void RemoveNodeWithOneChild(Pointer node);
  void RemoveNodeWithTwoChild(Pointer node);
  void RebalanceTree(Pointer node);
  bool NodeIsLeftChild(Pointer node);
  Pointer MinValueFromRight(const Pointer node) const;
  void ExcludeNode(Pointer a, Pointer b);
};

template <typename Key, typename Value, typename ValueEqual>
SelfBalancingBinarySearchTree<Key, Value,
                              ValueEqual>::SelfBalancingBinarySearchTree()
    : leaf_(new Node()), root_(nullptr){};

template <typename Key, typename Value, typename ValueEqual>
SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::
    SelfBalancingBinarySearchTree(const SelfBalancingBinarySearchTree& other)
    : leaf_(new Node()), root_(other.root_){};

template <typename Key, typename Value, typename ValueEqual>
SelfBalancingBinarySearchTree<Key, Value, ValueEqual>&
SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::operator=(
    const SelfBalancingBinarySearchTree& other) {
  if (this != &other) {
    if (root_) delete root_;
    root_ = other.root_;
    leaf_ = other.leaf_;
  }
  return *this;
}

template <typename Key, typename Value, typename ValueEqual>
SelfBalancingBinarySearchTree<Key, Value,
                              ValueEqual>::~SelfBalancingBinarySearchTree() {
  Clear();
  delete leaf_;
}

template <typename Key, typename Value, typename ValueEqual>
bool SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Set(
    const Key& key, const Value& value) {
  Pointer tmp = new Node(key, value);
  if (!root_) {
    root_ = tmp;
    root_->color = Color::Black;
    root_->link[0] = root_->link[1] = leaf_;
  } else {
    if (Exists(key)) {
      delete tmp;
      return false;
    } else {
      Pointer current_node = root_;
      Pointer parent = root_;
      while (current_node != leaf_) {
        parent = current_node;
        current_node = (current_node->data.first > key) ? current_node->link[0]
                                                        : current_node->link[1];
      }
      current_node = tmp;
      current_node->parent = parent;
      current_node->link[0] = current_node->link[1] = leaf_;

      bool dir = parent->data.first > key;
      parent->link[!dir] = current_node;

      BalanceTree(current_node);
    }
  }

  return true;
}

template <typename Key, typename Value, typename ValueEqual>
Value SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Get(
    const Key& key) const {
  if (!root_) throw std::invalid_argument("Key is not exists");

  Pointer current_node = root_;
  while (current_node != leaf_) {
    if (current_node->data.first == key) return current_node->data.second;
    if (current_node->data.first > key)
      current_node = current_node->link[0];
    else
      current_node = current_node->link[1];
  }
  throw std::invalid_argument("Key is not exists");
}

template <typename Key, typename Value, typename ValueEqual>
bool SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Exists(
    const Key& key) const {
  if (Search(key)) return true;
  return false;
}

template <typename Key, typename Value, typename ValueEqual>
bool SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Del(
    const Key& key) {
  Pointer node = Search(key);
  if (!node) return false;
  RemoveNode(node);
  return true;
}

template <typename Key, typename Value, typename ValueEqual>
bool SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Update(
    const Key& key, const Value& value) {
  Pointer node = Search(key);
  if (!node) return false;
  node->data.second = value;
  return true;
}

template <typename Key, typename Value, typename ValueEqual>
std::vector<Key> SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Keys()
    const {
  std::vector<Key> keys;
  Iterator i = Begin();
  if (i.IsValid()) {
    while (i != End()) {
      keys.push_back(i->first);
      ++i;
    }
    keys.push_back(i->first);
  }
  return keys;
}

template <typename Key, typename Value, typename ValueEqual>
bool SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Rename(
    const Key& key, const Key& new_key) {
  Pointer node = Search(key);
  if (!node) return false;
  Set(new_key, node->data.second);
  RemoveNode(node);
  return true;
}

template <typename Key, typename Value, typename ValueEqual>
std::vector<Key> SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Find(
    const Value& value) const {
  std::vector<Key> keys;
  Iterator i = Begin();
  if (i.IsValid()) {
    ValueEqual comparator;
    while (i != End()) {
      if (comparator(i->second, value)) keys.push_back(i->first);
      ++i;
    }
    if (comparator(i->second, value)) keys.push_back(i->first);
  }
  return keys;
}

template <typename Key, typename Value, typename ValueEqual>
std::vector<Value>
SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Showall() const {
  std::vector<Value> nodes;
  Iterator i = Begin();
  if (i.IsValid()) {
    while (i != End()) {
      nodes.push_back(i->second);
      ++i;
    }
    nodes.push_back(i->second);
  }
  return nodes;
}

template <typename Key, typename Value, typename ValueEqual>
BSTNode<Key, Value>*
SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Search(
    const Key& key) const {
  Pointer node = root_;
  while (node && node != leaf_) {
    if (node->data.first == key) return node;
    node = (node->data.first > key) ? node->link[0] : node->link[1];
  }
  return nullptr;
}

template <typename Key, typename Value, typename ValueEqual>
bool SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::RemoveNode(
    Pointer node) {
  if (!node || node == leaf_) return false;
  switch (CountChildren(node)) {
    case 0:
      RemoveNodeWithoutChildren(node);
      break;
    case 1:
      RemoveNodeWithOneChild(node);
      break;
    case 2:
      RemoveNodeWithTwoChild(node);
      break;
  }
  return true;
}

template <typename Key, typename Value, typename ValueEqual>
int SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::CountChildren(
    Pointer node) const {
  int count = 0;
  if (node) {
    count += node->link[0] != leaf_;
    count += node->link[1] != leaf_;
  }
  return count;
}

template <typename Key, typename Value, typename ValueEqual>
void SelfBalancingBinarySearchTree<
    Key, Value, ValueEqual>::RemoveNodeWithoutChildren(Pointer node) {
  Pointer parent = GetParent(node);
  if (!parent) {
    root_ = nullptr;
  } else {
    bool dir = (parent->link[1] == node);
    parent->link[dir] = leaf_;
  }
  delete node;
}

template <typename Key, typename Value, typename ValueEqual>
void SelfBalancingBinarySearchTree<
    Key, Value, ValueEqual>::RemoveNodeWithOneChild(Pointer node) {
  Pointer child = GetChild(node);
  bool child_color = child->color;
  Swap(node, child);
  node->link[0] = child->link[0];
  node->link[1] = child->link[1];
  if (child->link[0] != leaf_) child->link[0]->parent = node;
  if (child->link[1] != leaf_) child->link[1]->parent = node;

  delete child;

  if (child_color)
    node->color = Color::Black;
  else
    RebalanceTree(node);
}

template <typename Key, typename Value, typename ValueEqual>
void SelfBalancingBinarySearchTree<
    Key, Value, ValueEqual>::RemoveNodeWithTwoChild(Pointer node) {
  Pointer min_node = MinValueFromRight(node);
  bool color = min_node->color;
  Pointer child = GetChild(min_node);
  Pointer parent = GetParent(min_node);

  if (parent == node) {
    child->parent = min_node;
  } else {
    ExcludeNode(min_node, child);
    min_node->link[1] = node->link[1];
    min_node->link[1]->parent = min_node;
  }

  ExcludeNode(node, min_node);
  min_node->link[0] = node->link[0];
  min_node->link[0]->parent = min_node;
  min_node->color = node->color;

  delete node;
  if (color == Color::Black) RebalanceTree(child);
  if (child == leaf_) child->parent = nullptr;
}

template <typename Key, typename Value, typename ValueEqual>
BSTNode<Key, Value>*
SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::GetParent(
    Pointer node) const {
  return node->parent;
}

template <typename Key, typename Value, typename ValueEqual>
BSTNode<Key, Value>*
SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::GetChild(
    Pointer node) const {
  Pointer child = node->link[0] != leaf_ ? node->link[0] : node->link[1];
  return child;
}

template <typename Key, typename Value, typename ValueEqual>
void SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Swap(Pointer a,
                                                                 Pointer b) {
  std::swap(a->data.first, b->data.first);
  std::swap(a->data.second, b->data.second);
}

template <typename Key, typename Value, typename ValueEqual>
void SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::RebalanceTree(
    Pointer node) {
  while (node != root_ && node->color == Color::Black) {
    Pointer parent = GetParent(node);
    Pointer brother;
    bool dir = NodeIsLeftChild(node);

    brother = parent->link[dir];
    if (brother != leaf_ && brother->color == Color::Red) {
      brother->color = Color::Black;
      parent->color = Color::Red;
      if (dir)
        LeftRotation(parent);
      else
        RightRotation(parent);
      brother = parent->link[dir];
    }
    if (brother != leaf_ && brother->link[!dir]->color == Color::Black &&
        brother->link[dir]->color == Color::Black) {
      brother->color = Color::Red;
      node = parent;
    } else {
      if (brother != leaf_ && brother->link[dir]->color == Color::Black) {
        brother->link[!dir]->color = Color::Black;
        brother->color = Color::Red;
        if (dir)
          RightRotation(brother);
        else
          LeftRotation(brother);

        brother = parent->link[dir];
      }
      brother->color = parent->color;
      parent->color = Color::Black;
      if (brother != leaf_) brother->link[dir]->color = Color::Black;
      if (dir)
        LeftRotation(parent);
      else
        RightRotation(parent);
      node = root_;
    }
  }
  node->color = Color::Black;
}

template <typename Key, typename Value, typename ValueEqual>
bool SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::NodeIsLeftChild(
    Pointer node) {
  return GetParent(node)->link[0] == node;
}

template <typename Key, typename Value, typename ValueEqual>
void SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::LeftRotation(
    Pointer node) {
  Pointer tmp = node->link[1];
  node->link[1] = tmp == leaf_ ? leaf_ : tmp->link[0];
  if (tmp != leaf_) {
    tmp->link[0]->parent = node;
  }
  tmp->parent = node->parent;
  if (node->parent == nullptr) {
    root_ = tmp;
  } else {
    bool dir = NodeIsLeftChild(node);
    node->parent->link[!dir] = tmp;
  }
  tmp->link[0] = node;
  node->parent = tmp;
}

template <typename Key, typename Value, typename ValueEqual>
void SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::RightRotation(
    Pointer node) {
  Pointer tmp = node->link[0];
  node->link[0] = tmp == leaf_ ? leaf_ : tmp->link[1];
  if (tmp != leaf_) {
    tmp->link[1]->parent = node;
  }
  tmp->parent = node->parent;
  if (node->parent == nullptr) {
    root_ = tmp;
  } else {
    bool dir = NodeIsLeftChild(node);
    node->parent->link[!dir] = tmp;
  }
  tmp->link[1] = node;
  node->parent = tmp;
}

template <typename Key, typename Value, typename ValueEqual>
BSTNode<Key, Value>*
SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::MinValueFromRight(
    const Pointer node) const {
  Pointer tmp = node->link[1];
  while (tmp->link[0] != leaf_) {
    tmp = tmp->link[0];
  }
  return tmp;
}

template <typename Key, typename Value, typename ValueEqual>
void SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::ExcludeNode(
    Pointer a, Pointer b) {
  Pointer parent = GetParent(a);
  if (!parent)
    root_ = b;
  else {
    bool dir = NodeIsLeftChild(a);
    parent->link[!dir] = b;
  }
  b->parent = parent;
}

template <typename Key, typename Value, typename ValueEqual>
void SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::BalanceTree(
    Pointer node) {
  while (node->parent != nullptr && node->parent->color == Color::Red) {
    bool dir = NodeIsLeftChild(node->parent);
    Pointer uncle = node->parent->parent->link[dir];
    if (uncle && uncle->color == Color::Red) {
      uncle->color = Color::Black;
      node->parent->color = Color::Black;
      node->parent->parent->color = Color::Red;
      node = node->parent->parent;
    } else {
      if (node == node->parent->link[dir]) {
        node = node->parent;
        if (dir)
          LeftRotation(node);
        else
          RightRotation(node);
      }
      node->parent->color = Color::Black;
      node->parent->parent->color = Color::Red;
      if (dir)
        RightRotation(node->parent->parent);
      else
        LeftRotation(node->parent->parent);
    }
  }
  root_->color = Color::Black;
}

template <typename Key, typename Value, typename ValueEqual>
typename SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Iterator
SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Begin() const {
  Pointer it = root_;
  while (it && it->link[0] != leaf_) {
    it = it->link[0];
  }
  return Iterator(it, leaf_);
}

template <typename Key, typename Value, typename ValueEqual>
typename SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Iterator
SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::End() const {
  Pointer it = root_;
  while (it && it->link[1] != leaf_) {
    it = it->link[1];
  }
  return Iterator(it, leaf_);
}

template <typename Key, typename Value, typename ValueEqual>
void SelfBalancingBinarySearchTree<Key, Value, ValueEqual>::Clear() {
  while (root_) {
    Pointer node = Begin().current_;
    RemoveNode(node);
  }
}

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_BST_SELF_BALANCING_BINARY_SEARCH_TREE_H_
