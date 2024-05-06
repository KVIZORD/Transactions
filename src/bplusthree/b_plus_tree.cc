

#include "b_plus_tree.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>


int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> uniform_dist(1, 100);
    std::vector<int> v;
    for (int i = 0; i < 20000; ++i) {
        auto el = uniform_dist(gen);
        if (std::find(v.begin(), v.end(), el) == v.end()) {
            v.push_back(el);
        }
    }
    std::shuffle(v.begin(), v.end(), gen);

    BPlusTree<int, std::string> tree;

    for (size_t i = 0; i < v.size(); ++i) {
        tree.Set(v[i], std::to_string(v[i]));
    }
    for (size_t i = 0; i < v.size(); ++i) {
        auto val = tree.Update(v[i], "-");
        std::cout << std::boolalpha << val << std::endl;
    }
    auto val = tree.Update(-7, "-");
    std::cout << std::boolalpha << val << std::endl;
    // for (size_t i = 0; i < v.size(); ++i) {
    //     // auto k = std::to_string(v[i]);
    //     // auto val = tree.Rename(v[i], -v[i]);
    //     std::cout << std::boolalpha << val << std::endl;
    // }
    auto keys = tree.Keys();
    for (auto el : keys) {
        std::cout << ">>> " << el << std::endl;
    }
    auto values = tree.Keys();
    for (auto el : values) {
        std::cout << ">>> " << el << std::endl;
    }
    return 0;
}
