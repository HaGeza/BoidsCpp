#ifndef TYPEDEFS_HPP
#define TYPEDEFS_HPP

#include <map>
#include <unordered_set>
#include <vector>

using uint = unsigned int;

template <typename T>
using vec = std::vector<T>;

using str = std::string;

template <typename T>
using uset = std::unordered_set<T>;

template <typename K, typename V>
using umap = std::unordered_map<K, V>;

using dd = std::pair<double, double>;
using ddd = std::tuple<double, double, double>;

#endif  // TYPEDEFS_HPP
