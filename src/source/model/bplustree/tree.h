#ifndef TRANSACTIONS_SOURCE_MODEL_BPLUSTREE_TREE_H_
#define TRANSACTIONS_SOURCE_MODEL_BPLUSTREE_TREE_H_

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

namespace s21 {

template <typename Key, typename Value>
class Tree {
 public:
  struct Node {
    bool is_leaf{true};
    Node* parent{};
    std::vector<Key> keys{};
    std::vector<Node*> children{};
    std::vector<Value> values{};
    Node* left{};
    Node* right{};

    explicit Node(size_t degree, bool is_leaf = true) : is_leaf(is_leaf) {
      keys.reserve(degree);
      if (!is_leaf) {
        children.reserve(degree + 1);
      } else {
        values.reserve(degree);
      }
    }
  };

  static constexpr size_t kDefaultDegree = 100;

  class Iterator {
   public:
    explicit Iterator(Node* node, size_t ind = 0) : node_(node), ind_(ind){};

    Iterator& operator++() noexcept {
      if (node_ == nullptr) {
        return *this;
      }
      ind_ += 1;
      if (ind_ >= node_->keys.size()) {
        node_ = node_->right;
        ind_ = 0;
      }
      return *this;
    }

    bool operator!=(const Iterator& other) const {
      return node_ != other.node_ || ind_ != other.ind_;
    }

    bool operator==(const Iterator& other) const {
      return node_ == other.node_ && ind_ == other.ind_;
    }

    std::pair<const Key&, Value&> operator*() const {
      return {node_->keys[ind_], node_->values[ind_]};
    }

   private:
    Node* node_;
    size_t ind_;
  };

  Tree() {}

  explicit Tree(size_t degree) noexcept : degree_(degree) {}

  Tree(const Tree& other) : degree_(other.degree_) {
    for (auto it = other.Begin(); it != other.End(); ++it) {
      Insert((*it).first, (*it).second);
    }
  }

  Tree(Tree&& other) noexcept
      : degree_(other.degree_),
        root_(std::move(other.root_)),
        begin_(std::move(other.begin_)) {
    other.root_ = nullptr;
    other.begin_ = nullptr;
  }

  ~Tree() { Clear(root_); }

  bool Exists(const Key& key) const {
    Node* leaf = SearchLeaf(key);
    if (leaf == nullptr) {
      return false;
    }
    auto it = std::lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
    return it != leaf->keys.end() && *it == key;
  }

  bool Insert(const Key& key, const Value& value) {
    Node* leaf = SearchLeaf(key);
    if (leaf == nullptr) {
      root_ = new Node(degree_, true);
      root_->is_leaf = true;
      root_->keys.push_back(key);
      root_->values.push_back(value);
      begin_ = root_;
      return true;
    }
    auto middle_it =
        std::lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
    if (middle_it != leaf->keys.end() && *middle_it == key) {
      return false;
    };
    size_t insert_index = std::distance(leaf->keys.begin(), middle_it);

    if (middle_it == leaf->keys.end()) {
      leaf->values.push_back(value);
      leaf->keys.push_back(key);
    } else if (*middle_it == key) {
      leaf->values[insert_index] = value;
    } else {
      leaf->values.push_back(value);
      leaf->keys.push_back(key);
      std::rotate(leaf->keys.begin() + insert_index, leaf->keys.end() - 1,
                  leaf->keys.end());
      std::rotate(leaf->values.begin() + insert_index, leaf->values.end() - 1,
                  leaf->values.end());
    }
    if (leaf->keys.size() > 2 * degree_) {
      Split(leaf);
    }
    return true;
  }

  bool Remove(const Key& key) {
    Node* leaf = SearchLeaf(key);
    if (leaf == nullptr) {
      return false;
    }
    return RemoveKey(leaf, key);
  }

  const Value& Search(const Key& key) const {
    Node* leaf = SearchLeaf(key);
    if (leaf == nullptr) {
      throw std::invalid_argument("Key not found");
    }
    auto it = std::lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
    if (it == leaf->keys.end() || *it != key) {
      throw std::invalid_argument("Key not found");
    }
    size_t index = std::distance(leaf->keys.begin(), it);
    return leaf->values[index];
  }

  bool Update(const Key& key, const Value& value) {
    Node* leaf = SearchLeaf(key);
    if (leaf == nullptr) {
      return false;
    }
    auto it = std::lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
    if (it == leaf->keys.end() || *it != key) {
      return false;
    }
    size_t index = std::distance(leaf->keys.begin(), it);
    leaf->values[index] = value;
    return true;
  }

  Iterator Begin() const { return Iterator(begin_); }

  Iterator End() const { return Iterator(nullptr); }

 private:
  size_t degree_ = kDefaultDegree;
  Node* root_{};
  Node* begin_{};

  void Clear(Node* node) {
    if (node == nullptr) {
      return;
    }
    for (auto child : node->children) {
      Clear(child);
    }
    delete node;
  }

  void BorrowFromLeftNeighbor(Node* left, Node* right) {
    right->keys.insert(right->keys.begin(), left->keys.back());
    left->keys.pop_back();

    if (right->is_leaf) {
      right->values.insert(right->values.begin(), left->values.back());
      left->values.pop_back();
    } else {
      Node* borrowed = left->children.back();
      right->children.insert(right->children.begin(), borrowed);
      borrowed->parent = right;
      left->children.pop_back();
    }
    RecursiveUpdateKeys(right->parent);
  }

  void BorrowFromRightNeighbor(Node* left, Node* right) {
    left->keys.push_back(right->keys.front());
    right->keys.erase(right->keys.begin());

    if (left->is_leaf) {
      left->values.push_back(right->values.front());
      right->values.erase(right->values.begin());
    } else {
      Node* borrowed = right->children.front();
      left->children.push_back(borrowed);
      borrowed->parent = left;
      right->children.erase(right->children.begin());
    }
    RecursiveUpdateKeys(right->parent);
  }

  void MergeNodes(Node* left, Node* right) {
    if (left->is_leaf) {
      std::move(right->keys.begin(), right->keys.end(),
                std::back_inserter(left->keys));
      std::move(right->values.begin(), right->values.end(),
                std::back_inserter(left->values));
    } else {
      for (auto& item : right->children) {
        left->keys.push_back(item->keys.front());
      }
      std::move(right->children.begin(), right->children.end(),
                std::back_inserter(left->children));
      for (auto& item : left->children) {
        item->parent = left;
      }
    }
    if (right->right != nullptr) {
      right->right->left = left;
    }
    left->right = right->right;
    for (size_t i = 0; i < right->parent->children.size(); ++i) {
      Node* item = right->parent->children[i];
      if (item == right) {
        RemoveKey(right->parent, right->parent->keys[i - 1]);
        break;
      }
    }
    UpdateKeys(left->parent);
    delete right;
  }

  void MoveDataToNewNode(Node* src, Node* dst, size_t border) {
    if (src->is_leaf) {
      std::move(src->keys.begin() + border, src->keys.end(),
                std::back_inserter(dst->keys));
      std::move(src->values.begin() + border, src->values.end(),
                std::back_inserter(dst->values));
      src->keys.erase(src->keys.begin() + border, src->keys.end());
      src->values.erase(src->values.begin() + border, src->values.end());
    } else {
      std::move(src->keys.begin() + border + 1, src->keys.end(),
                std::back_inserter(dst->keys));
      src->keys.erase(src->keys.begin() + border, src->keys.end());
      std::move(src->children.begin() + border + 1, src->children.end(),
                std::back_inserter(dst->children));
      for (auto& item : dst->children) {
        item->parent = dst;
      }
      src->children.erase(src->children.begin() + border + 1,
                          src->children.end());
    }
  }

  void Rebalance(Node* node) {
    Node* left_neighbor = node->left;
    Node* right_neighbor = node->right;

    if (left_neighbor != nullptr && left_neighbor->keys.size() > degree_) {
      BorrowFromLeftNeighbor(left_neighbor, node);
    } else if (right_neighbor != nullptr &&
               right_neighbor->keys.size() > degree_) {
      BorrowFromRightNeighbor(node, right_neighbor);
    } else if (left_neighbor && left_neighbor->parent == node->parent) {
      MergeNodes(left_neighbor, node);
    } else if (right_neighbor && right_neighbor->parent == node->parent) {
      MergeNodes(node, right_neighbor);
    }
  }

  void RecursiveUpdateKeys(Node* node) {
    while (node) {
      UpdateKeys(node);
      node = node->parent;
    }
  }

  bool RemoveKey(Node* node, const Key& key) {
    bool is_removed = RemoveKeyFromNode(node, key);

    if (node == root_) {
      UpdateRoot(node);
      return is_removed;
    }
    if (node->keys.size() < degree_) {
      Rebalance(node);
    } else {
      RecursiveUpdateKeys(node->parent);
    }
    return is_removed;
  }

  bool RemoveKeyFromNode(Node* node, const Key& key) {
    auto it = std::lower_bound(node->keys.begin(), node->keys.end(), key);
    size_t index = std::distance(node->keys.begin(), it);
    if (it == node->keys.end() || node->keys.size() == 0) {
      return false;
    }
    node->keys.erase(it);
    if (node->is_leaf) {
      node->values.erase(node->values.begin() + index);
    } else {
      node->children.erase(node->children.begin() + index + 1);
    }
    return true;
  }

  Node* SearchLeaf(const Key& key) const {
    if (root_ == nullptr) {
      return nullptr;
    }
    Node* node = root_;
    while (!node->is_leaf) {
      auto it = std::upper_bound(node->keys.begin(), node->keys.end(), key);
      if (it == node->keys.end()) {
        node = node->children.back();
      } else if (it == node->keys.begin() && key < node->keys.front()) {
        node = node->children.front();
      } else {
        size_t index = std::distance(node->keys.begin(), it);
        node = node->children[index];
      }
    }
    return node;
  }

  const Key& SearchMinKey(Node* node) const {
    while (!node->is_leaf) {
      node = node->children[0];
    }
    return node->keys[0];
  }

  void Split(Node* node) {
    Node* new_node = new Node(degree_, node->is_leaf);
    new_node->left = node;
    new_node->right = node->right;
    if (node->right) {
      node->right->left = new_node;
    }
    node->right = new_node;
    Key mid_key = node->keys[degree_];
    new_node->is_leaf = node->is_leaf;
    new_node->parent = node->parent;
    MoveDataToNewNode(node, new_node, degree_);
    if (node == root_) {
      Node* new_root = new Node(degree_, false);
      new_root->is_leaf = false;
      new_root->keys.push_back(mid_key);
      new_root->children.push_back(node);
      new_root->children.push_back(new_node);
      root_ = new_root;
      node->parent = new_root;
      new_node->parent = new_root;
      return;
    }

    Node* parent = node->parent;
    auto mid_it = std::lower_bound(parent->keys.begin(), parent->keys.end(),
                                   new_node->keys.front());
    size_t mid_index = std::distance(parent->keys.begin(), mid_it);

    parent->keys.push_back(mid_key);
    parent->children.push_back(new_node);
    std::rotate(parent->keys.begin() + mid_index, parent->keys.end() - 1,
                parent->keys.end());
    std::rotate(parent->children.begin() + mid_index + 1,
                parent->children.end() - 1, parent->children.end());

    if (parent->keys.size() > 2 * degree_) {
      Split(parent);
    }
  }

  void UpdateRoot(Node* node) {
    if (node->keys.size() > 0) {
      return;
    }
    if (!node->is_leaf) {
      root_ = node->children[0];
      root_->parent = nullptr;
    } else {
      root_ = nullptr;
    }
    delete node;
  }

  void UpdateKeys(Node* node) {
    if (node == nullptr) {
      return;
    }
    for (size_t i = 0; i < node->keys.size(); ++i) {
      Key key = SearchMinKey(node->children[i + 1]);
      node->keys[i] = key;
    }
  }
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_BPLUSTREE_TREE_H_