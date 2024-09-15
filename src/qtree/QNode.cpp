#include "qtree/QNode.hpp"

#include <iostream>

#include "doctest.h"

QNode::QNode(double x, double y, double size, uint maxEntries)
    : x(x),
      y(y),
      size(size),
      maxEntries(maxEntries),
      entryIndices(),
      children(nullptr),
      numEntries(0) {}

void QNode::pickChild(size_t entryIndex, umap<size_t, dd>& positions) {
    dd position = positions[entryIndex];
    double halfSize = size / 2;
    if (position.first < x + halfSize) {
        if (position.second < y + halfSize) {
            children[0].insert(entryIndex, positions);
        } else {
            children[2].insert(entryIndex, positions);
        }
    } else {
        if (position.second < y + halfSize) {
            children[1].insert(entryIndex, positions);
        } else {
            children[3].insert(entryIndex, positions);
        }
    }
}

void QNode::insert(size_t entryIndex, umap<size_t, dd>& positions) {
    numEntries++;

    if (entryIndices.size() < maxEntries) {
        entryIndices.push_back(entryIndex);
        return;
    }

    if (!children) {
        double halfSize = size / 2;
        children = std::make_unique<QNode[]>(4);
        children[0] = QNode(x, y, halfSize, maxEntries);
        children[1] = QNode(x + halfSize, y, halfSize, maxEntries);
        children[2] = QNode(x, y + halfSize, halfSize, maxEntries);
        children[3] = QNode(x + halfSize, y + halfSize, halfSize, maxEntries);
    }

    pickChild(entryIndex, positions);
    for (size_t index : entryIndices) {
        pickChild(index, positions);
    }
    entryIndices.clear();
}

size_t QNode::getNumEntries() const { return numEntries; }

const QNode* QNode::getChild(size_t index) const {
    if (!children) {
        return nullptr;
    }
    return &children[index];
}

// Doctests

TEST_CASE("QNode::insert") {
    QNode node(0.0, 0.0, 10.0, 1);

    SUBCASE("Inserting a single point") {
        umap<size_t, dd> positions = {{0, {5.0, 5.0}}};
        node.insert(0, positions);
        CHECK(node.getNumEntries() == 1);
    }

    SUBCASE("Inserting multiple points") {
        umap<size_t, dd> positions = {
            {0, {5.0, 5.0}}, {1, {2.0, 5.0}}, {2, {2.0, 2.1}}, {3, {1.2, 3.9}}};

        for (size_t i = 0; i < positions.size(); i++) {
            node.insert(i, positions);
        }

        CHECK(node.getNumEntries() == 4);
        CHECK(node.getChild(0)->getNumEntries() == 2);
        CHECK(node.getChild(1)->getNumEntries() == 0);
        CHECK(node.getChild(2)->getNumEntries() == 1);
        CHECK(node.getChild(3)->getNumEntries() == 1);

        CHECK(node.getChild(0)->getChild(0)->getNumEntries() == 1);
        CHECK(node.getChild(0)->getChild(1)->getNumEntries() == 0);
        CHECK(node.getChild(0)->getChild(2)->getNumEntries() == 1);
        CHECK(node.getChild(0)->getChild(3)->getNumEntries() == 0);

        for (size_t i = 1; i < 4; i++) {
            for (size_t j = 0; j < 4; j++) {
                CHECK(node.getChild(i)->getChild(j) == nullptr);
            }
        }
    }
}