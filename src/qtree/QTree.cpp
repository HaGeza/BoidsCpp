#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "qtree/QTree.hpp"

#include <iostream>

#include "doctest.h"

// QTree
QTree::QTree(double size, uint maxEntriesPerNode)
    : size(size), root(0, 0, size, maxEntriesPerNode), positions() {}

void QTree::insert(double x, double y, size_t index) {
    // TODO: Throw exception and write tests
    if (positions.find(index) != positions.end()) {
        std::cerr << "Index " << index << " already exists in QTree\n";
        return;
    }
    if (x < 0 || x >= size || y < 0 || y >= size) {
        std::cerr << "Position (" << x << ", " << y
                  << ") is out of bounds for QTree of size " << size << "\n";
        return;
    }

    positions[index] = {x, y};
    root.insert(index, positions);
}

umap<size_t, uset<size_t>> QTree::getNeighborhoods(double r) const {
    return root.getNeighborhoods(r, positions);
}

size_t QTree::getNumEntries() const { return positions.size(); }

// Doctests

TEST_CASE("QTree::insert") {
    QTree tree(10.0, 1);

    SUBCASE("Inserting a single point") {
        tree.insert(5.0, 5.0, 0);
        CHECK(tree.getNumEntries() == 1);
    }

    SUBCASE("Insert multiple points") {
        tree.insert(5.0, 5.3, 0);
        tree.insert(2.0, 5.0, 1);
        tree.insert(2.0, 2.1, 2);
        CHECK(tree.getNumEntries() == 3);
    }
}
