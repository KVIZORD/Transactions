#include "tree.h"


template <typename Key, typename Value>
class BPlusTree {
public:
    template <typename T>
    struct NewValue {
        int ttl;
        T value;
    };
    using new_value = NewValue<Value>;

    ~BPlusTree() = default;

    bool Set(const Key& key, const Value& value, int validity = 0) {
        return tree.insert(key, new_value{validity, value});
    };

    Value Get(const Key& key) const {
        const new_value& value = tree.search(key);
        return value.value;
    };

    bool Exists(const Key& key) const {
        return tree.Exists(key);
    };

    bool Del(const Key& key) {
        return tree.remove(key);
    };

    bool Update(const Key& key, const Value& value) {
        try {
            const new_value& old = tree.search(key);
            return tree.Update(key, new_value{old.ttl, value});
        } catch(const std::invalid_argument& err) {
            return false;
        }
    };

    std::vector<Key> Keys()  {
        std::vector<Key> keys;
        for (auto it = tree.Begin(); it != tree.End(); ++it) {
            keys.push_back((*it).first);
        }
        return keys;
    };

    bool Rename(const Key& key, const Key& new_key) {
        if (tree.Exists(new_key)) {
            return false;
        }
        const new_value value = tree.search(key);
        bool res = tree.remove(key);
        if (!res) {
            return false;        
        }
        return tree.insert(new_key, value);
    };

    int Ttl(const Key& key) const {
        const new_value& value = tree.search(key);
        return value.ttl;
    };

    // std::vector<Key> Find(const Value& value) const {

    // };

    std::vector<Value> Showall() const {
        std::vector<Value> values;
        for (auto it = tree.Begin(); it != tree.End(); ++it) {
            values.push_back((*it).second.value);
        }
        return values;
    };

    // std::pair<bool, int> Upload(const std::string& path) {

    // };

    // std::pair<bool, int> Export(const std::string& path) const {

    // };

    void Print() {
        tree.printTree();
    }
private:
    Three<Key, new_value> tree;

};
