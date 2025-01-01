#pragma once

#include <cstddef>
#include <array>
#include <bitset>

template<std::size_t NumSets, std::size_t NumWays>
class TreePlru {
    using TreeT = std::bitset<NumWays>;
public:
    TreePlru() = default;

    std::size_t get_victim(std::size_t set_index);
    void touch(std::size_t set_index, std::size_t way);
private:
    std::array<TreeT, NumSets> plru_trees;
};

//             1                 x
//         2       3        2*x    2*x + 1 
//       4   5   6   7
//      8 91011 12131415   8-15  => tree_iter - 8(NumWays) = way
// way= 0 1 2 3  4 5 6 7   0-7   =>    way            
template<std::size_t NumSets, std::size_t NumWays>
std::size_t TreePlru<NumSets, NumWays>::get_victim(std::size_t set_index) {
    auto &tree = plru_trees[set_index];
    std::size_t tree_iter = 1;
    while(tree_iter < NumWays) {
        bool to_right = tree[tree_iter];
        tree_iter *= 2;
        if (to_right)
            ++tree_iter;
    }
    return tree_iter - NumWays;
}

// way= 0 1 2 3  4 5 6 7         x
//      8 91011 12131415      x += 8(NumWays)
//       4   5   6   7      new_x = x / 2        
//         2       3
//             1
// 
//      4   5  &1 => 0 1    (from)0      (from)1
//        2           2    set(to 1)   reset(to 0)
template<std::size_t NumSets, std::size_t NumWays>
void TreePlru<NumSets, NumWays>::touch(
    std::size_t set_index, std::size_t way_index) {
    auto &tree = plru_trees[set_index];
    for(std::size_t tree_iter = way_index + NumWays, next_iter;
        tree_iter != 1; tree_iter = next_iter) {
        next_iter = tree_iter / 2;
        tree_iter & 1 ?
            tree.reset(next_iter) : tree.set(next_iter);
    }
}