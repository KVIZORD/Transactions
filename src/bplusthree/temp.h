#include <vector>
#include <string>
#include <iostream>
#include <algorithm>


template <typename Key, typename Value>
struct Node {
    bool is_leaf = true;
    Node *parent;
    std::vector<Key> keys;
    std::vector<Node*> childs;
    std::vector<Value> values;
    Node *left;
    Node *right;
};


template <typename Key, typename Value>
class Three {
public:
    using node_type = Node<Key, Value>;
    Three() {}
    ~Three() {}

    node_type* root() const {
        return root_;
    }

    node_type* searchLeaf(Key key) {
        if (root_ == nullptr) {
            return nullptr;
        }
        node_type* node = root_;
        while (!node->is_leaf) {
            // std::cout << "keys size = " << node->keys.size() << std::endl;
            for (size_t i = 0; i <= node->keys.size(); ++i) {
                if (i == node->keys.size() || key < node->keys[i]) {
                    // std::cout << i << std::endl;
                    node = node->childs[i];
                    break;
                }
            }
        }
        return node;
    }

    void insert(Key key, Value value) {
        node_type* leaf = searchLeaf(key);
        if (leaf == nullptr) {
            root_ = new node_type;
            root_->is_leaf = true;
            root_->keys.push_back(key);
            root_->values.push_back(value);
            return;
        }

        auto middle_it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
        size_t insert_index = middle_it - leaf->keys.begin();

        if (middle_it == leaf->keys.end()) {
            leaf->values.push_back(value);
            leaf->keys.push_back(key);
        } else if (*middle_it == key) {
            leaf->values[insert_index] = value;
        } else {
            leaf->values.push_back(value);
            leaf->keys.push_back(key);            
            std::rotate(leaf->keys.begin() + insert_index, leaf->keys.end() - 1, leaf->keys.end());
            std::rotate(leaf->values.begin() + insert_index, leaf->values.end() - 1, leaf->values.end());
        }
        if (leaf->keys.size() > 2 * degree) {
            split(leaf);
        }
    }

    void remove(Key key) {
        node_type* leaf = searchLeaf(key);
        if (leaf == nullptr) {
            std::cout << "key not found" << std::endl;
            return;
        }
        removeKey(leaf, key);
    }

    size_t degree{2};
    node_type *root_{};
    node_type *begin_leaf{};
private:
    void removeKey(node_type *node, Key& key) {
        std::cout << "remove key = " << key << std::endl;
        // printNode(node);
        // printTree();
        // удаление ключа из узла
        removeKeyFromNode(node, key);
        // printTree();
        // printNode(node);
        // если узел является конем
        if (node == root_) {
            // std::cout 
            // TODO ("Remove root node if it is empty");
            // std::cout << "Remove root node if it is empty: " << key << std::endl;
            updateRoot(node);
            return;
        }
        // printTree();
        // если узел заполнен не оптимально
        if (node->keys.size() < degree) {
            // std::cout << "remove key and rebalance = " << key << std::endl;
            rebalance(node);
        } else {
            recursiveUpdateKeys(node->parent);
        }
    }

    void rebalance(node_type* node) {
        // std::cout << "============== REBALANCE ==============" << std::endl;
        node_type* left_neighbor = node->left;
        node_type* right_neighbor = node->right;
        // printTree();
        if (left_neighbor != nullptr && left_neighbor->keys.size() > degree) {
            // Заимствование из левого узла
            std::cout << "Заимствование из левого узла" << std::endl;
            node->keys.insert(node->keys.begin(), left_neighbor->keys.back());
            node->values.insert(node->values.begin(), left_neighbor->values.back());
            left_neighbor->keys.pop_back();
            left_neighbor->values.pop_back();
            recursiveUpdateKeys(node->parent);
        } else if (right_neighbor != nullptr && right_neighbor->keys.size() > degree) {
            // Заимствование из правого узла
            std::cout << "Заимствование из правого узла" << std::endl;
            node->keys.push_back(right_neighbor->keys.front());
            node->values.push_back(right_neighbor->values.front());
            right_neighbor->keys.erase(right_neighbor->keys.begin());
            right_neighbor->values.erase(right_neighbor->values.begin());
            recursiveUpdateKeys(right_neighbor->parent);
        } else if (left_neighbor && left_neighbor->parent == node->parent) {
            // std::cout << "============== REBALANCE ==============" << std::endl;
            // printTree();
            // Объединение с левым узлом
            // node->keys[0] = 1000;
            // printNode(node);
            // printNode(left_neighbor);

            std::cout << "Объединение с левым узлом" << std::endl;
            // TODO("Проверить узел или лист, если лист, то добавлять ключ");
            if (node->is_leaf) {
                std::move(node->keys.begin(), node->keys.end(), std::back_inserter(left_neighbor->keys));
                std::move(node->values.begin(), node->values.end(), std::back_inserter(left_neighbor->values));
            } else {
                std::transform(node->childs.begin(), node->childs.end(), std::back_inserter(left_neighbor->keys), [](auto item) { return item->keys.front(); });
                std::move(node->childs.begin(), node->childs.end(), std::back_inserter(left_neighbor->childs));
                for (auto& item : left_neighbor->childs) {
                    item->parent = left_neighbor;
                }
            }
            // node->keys.erase(node->keys.begin(), node->keys.end());
            // node->childs.erase(node->childs.begin(), node->childs.end());
            if (node->right != nullptr) {
                node->right->left = left_neighbor;
            }
            left_neighbor->right = node->right;
            // left_neighbor.end()
            // printTree();
            auto it = upper_bound(node->parent->keys.begin(), node->parent->keys.end(), *left_neighbor->keys.begin());
            std::cout << "Remove from parent key = " << *it << std::endl;
            removeKey(node->parent, *it);
            // std::cout << "Remove node = " << node << std::endl;
            delete node;
        } else if (right_neighbor && right_neighbor->parent == node->parent) {
            // Объединение с правым узлом
            std::cout << "Объединение с правым узлом" << std::endl;
            if (node->is_leaf) {
                std::move(right_neighbor->keys.begin(), right_neighbor->keys.end(), std::back_inserter(node->keys));
                std::move(right_neighbor->values.begin(), right_neighbor->values.end(), std::back_inserter(node->values));
            } else {
                std::transform(right_neighbor->childs.begin(), right_neighbor->childs.end(), std::back_inserter(node->keys), [](auto item) { return item->keys.front(); });
                std::move(right_neighbor->childs.begin(), right_neighbor->childs.end(), std::back_inserter(node->childs));
                for (auto& item : node->childs) {
                    item->parent = node;
                }
            }
            
            if (right_neighbor->right != nullptr) {
                right_neighbor->right->left = node;
            }
            node->right = right_neighbor->right;
            // printTree();
            auto it = upper_bound(right_neighbor->parent->keys.begin(), right_neighbor->parent->keys.end(), *node->keys.begin());
            removeKey(right_neighbor->parent, *it);
            delete right_neighbor;
        }
    }


    void updateRoot(node_type *node) {
        if (node->keys.size() > 0) {
            return;
        }
        if (!node->is_leaf) {
            root_ = node->childs[0];
            root_->parent = nullptr;
        } else {
            root_ = nullptr;
        }
        delete node;
    }

    void recursiveUpdateKeys(node_type* node) {
        while (node) {
            updateKeys(node);
            node = node->parent;
        }
    }

    void updateKeys(node_type* node) {
        for (size_t i = 0; i < node->keys.size(); ++i) {
            Key key = searchMinKey(node->childs[i + 1]);
            node->keys[i] = key;
        }
    }

    Key searchMinKey(node_type* node) {
        while (!node->is_leaf) {
            node = node->childs[0];
        }
        return node->keys[0];
    }

    void mergeNode() {

    }

    void split(node_type* node) {
        std::cout << "split node = " << node << std::endl;

        // Создание нового узла и копирование в него половиы данных
        node_type* new_node = new node_type;
        new_node->left = node;
        new_node->right = node->right;
        if (node->right) {
            node->right->left = new_node;
        }
        node->right = new_node;
        
        Key mid_key = node->keys[degree];

        // Перемещение части данных в новый узел
        new_node->is_leaf = node->is_leaf;
        if (node->is_leaf) {
            moveDataToNewNode(node, new_node, degree);
        } else {
            moveDataToNewNode(node, new_node, degree + 1);
            node->keys.pop_back();
        }

        new_node->parent = node->parent;

        if (node == root_) {
            node_type *new_root = new node_type;
            new_root->is_leaf = false;
            new_root->keys.push_back(mid_key);
            new_root->childs.push_back(node);
            new_root->childs.push_back(new_node);
            
            root_ = new_root;
            node->parent = new_root;
            new_node->parent = new_root;
            return;
        }

        node_type* parent = node->parent;

        auto mid_it = lower_bound(parent->keys.begin(), parent->keys.end(), *new_node->keys.begin());
        size_t mid_index = mid_it - parent->keys.begin();
        parent->keys.push_back(mid_key);
        parent->childs.push_back(new_node);
        std::rotate(parent->keys.begin() + mid_index, parent->keys.end() - 1, parent->keys.end());
        std::rotate(parent->childs.begin() + mid_index + 1, parent->childs.end() - 1, parent->childs.end());
        // printNode(parent);

        if (parent->keys.size() > 2 * degree) {
            split(parent);
        }
    }

    void moveDataToNewNode(node_type *src, node_type *dst, size_t border) {

        // std::cout << "moveDataToNewNode src = " << src << ", dst = " << dst << ", border = " << border << std::endl;
        std::move(src->keys.begin() + border, src->keys.end(), std::back_inserter(dst->keys));
        src->keys.erase(src->keys.begin() + border, src->keys.end());
        if (src->is_leaf) {
            std::move(src->values.begin() + border, src->values.end(), std::back_inserter(dst->values));
            src->values.erase(src->values.begin() + border, src->values.end());
        } else {
            // std::transform(src->childs.begin() + border, src->childs.end(), src->childs.begin() + border, [dst](auto item) { return item->parent = dst; });
            // auto tmp = dst->childs.end() - 1;
            std::move(src->childs.begin() + border, src->childs.end(), std::back_inserter(dst->childs));
            for (auto& item : dst->childs) {
                item->parent = dst;
            }
            // std::transform(dst->childs.begin(), dst->childs.end(), dst->childs.begin(), [dst](auto item) { return item->parent = dst; });
            // std::transform(tmp, dst->childs.end(), dst, [dst](auto item) { return item->parent = dst; });
            // std::transform(src->childs.begin(), src->childs.end(), src->childs.begin(), [dst](auto item) { return item->parent = dst; });
            src->childs.erase(src->childs.begin() + border, src->childs.end());
        }
    }

    void removeKeyFromNode(node_type* node, Key& key) {
        auto it = lower_bound(node->keys.begin(), node->keys.end(), key);
        size_t index = it - node->keys.begin();
        std::cout << "index = " << index << std::endl;
        if (it == node->keys.end() || node->keys.size() == 0) {
            return;
        }
        // printNode(node);

        node->keys.erase(it);
        if (node->is_leaf) {
            node->values.erase(node->values.begin() + index);
        } else {
            node->childs.erase(node->childs.begin() + index + 1);
        }
    }

    void updateKeys(node_type *left, node_type *right) {
        node_type *parent_left = left->parent;
        node_type *parent_right = right->parent;

        while (parent_left != parent_right) {
            parent_left = parent_left->parent;
            parent_right = parent_right->parent;
        }
        
        if (parent_left == nullptr) {
            return;
        }
    }

 public:   
    void printNode(node_type *node) {
        std::cout << "========================================" << std::endl;
        std::cout << "Node: " << node << "(is leaf = " << node->is_leaf << ")" << std::endl;
        std::cout << "Parent: " << node->parent << std::endl;
        std::cout << "keys = ";
        for (size_t i = 0; i < node->keys.size(); ++i) {
            std::cout << " " << node->keys[i] << ",";
        }
        std::cout << std::endl;
        std::cout << "childs = ";
        for (size_t i = 0; i < node->childs.size(); ++i) {
            std::cout << " " << node->childs[i] << ",";
        }
        std::cout << std::endl;
        std::cout << "Left = " << node->left << ", Right = " << node->right << std::endl;
    }

    void printTree() {
        _printTree(root_);
    }

    void _printTree(node_type* node, int depth = 0) {
        for (int i = 0; i < depth; ++i) {
            std::cout << "|  ";
            // std::cout << "  ";
        }
        std::cout << "|__";

        for (const auto& key : node->keys) {
            std::cout << key << " ";
        }
        std::cout << std::endl;

        if (!node->is_leaf) {
            for (const auto& child : node->childs) {
                _printTree(child, depth + 1);
            }
        }
    }
};


