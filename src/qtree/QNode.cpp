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
      numEntries(0) {
    halfSize = size / 2;
}

void QNode::pickChild(size_t entryIndex, const umap<size_t, dd>& positions) {
    dd position = positions.at(entryIndex);
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

void QNode::insert(size_t entryIndex, const umap<size_t, dd>& positions) {
    numEntries++;

    if (!children) {
        if (entryIndices.size() < maxEntries) {
            entryIndices.push_back(entryIndex);
            return;
        }
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

umap<size_t, uset<size_t>> QNode::getNeighborhoods(
    double r, const umap<size_t, dd>& positions) const {
    double halfSize = size / 2, r2 = r * r;

    umap<size_t, uset<size_t>> neighborhoods;

    if (children) {
        vec<vec<size_t>> childEntries(4);

        for (size_t i = 0; i < 4; i++) {
            auto childNeighborhoods =
                children[i].getNeighborhoods(r, positions);

            for (auto& [entry, neighbors] : childNeighborhoods) {
                neighborhoods[entry] = neighbors;
                childEntries[i].emplace_back(entry);
            }
        }

        for (size_t i = 0; i < 4; i++) {
            for (auto entryI : childEntries[i]) {
                dd posI = positions.at(entryI);

                for (size_t j = i + 1; j < 4; j++) {
                    int sameRow = i / 2 == j / 2, sameCol = i % 2 == j % 2;
                    if (sameRow) {
                        if (abs(posI.first - (x + halfSize)) > r) {
                            continue;
                        }
                    } else if (sameCol) {
                        if (abs(posI.second - (y + halfSize)) > r) {
                            continue;
                        }
                    } else {
                        if ((pow(posI.first - (x + halfSize), 2) +
                             pow(posI.second - (y + halfSize), 2)) > r2) {
                            continue;
                        }
                    }

                    for (auto entryJ : childEntries[j]) {
                        dd posJ = positions.at(entryJ);
                        if ((pow(posI.first - posJ.first, 2) +
                             pow(posI.second - posJ.second, 2)) <= r2) {
                            neighborhoods[entryI].insert(entryJ);
                            neighborhoods[entryJ].insert(entryI);
                        }
                    }
                }
            }
        }
    } else {
        for (size_t entryIndex : entryIndices) {
            neighborhoods[entryIndex] = {};
        }

        for (size_t i = 0; i < entryIndices.size(); i++) {
            size_t entryI = entryIndices[i];
            dd posI = positions.at(entryI);

            for (size_t j = i + 1; j < entryIndices.size(); j++) {
                size_t entryJ = entryIndices[j];
                if (entryI == entryJ) {
                    continue;
                }

                dd posJ = positions.at(entryJ);
                if (sqrt(pow(posI.first - posJ.first, 2) +
                         pow(posI.second - posJ.second, 2)) <= r) {
                    neighborhoods[entryI].insert(entryJ);
                    neighborhoods[entryJ].insert(entryI);
                }
            }
        }
    }

    return neighborhoods;
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

TEST_CASE("QNode::getNeighborhoods") {
    QNode node(0.0, 0.0, 16.0, 1);

    SUBCASE("Single point") {
        umap<size_t, dd> positions = {{0, {5.0, 5.0}}};
        node.insert(0, positions);
        auto neighborhoods = node.getNeighborhoods(1.0, positions);
        CHECK(neighborhoods.size() == 1);
        CHECK(neighborhoods[0].size() == 0);
    }

    SUBCASE("Multiple points") {
        umap<size_t, dd> positions = {{0, {6, 6}},  {1, {10, 2}}, {2, {10, 6}},
                                      {3, {14, 6}}, {4, {5, 9}},  {5, {7, 11}},
                                      {6, {2, 14}}};

        for (size_t i = 0; i < positions.size(); i++) {
            node.insert(i, positions);
        }

        auto neighborhoods = node.getNeighborhoods(5.0, positions);

        CHECK(neighborhoods.size() == 7);
        CHECK(neighborhoods[0].size() == 2);
        CHECK(neighborhoods[1].size() == 1);
        CHECK(neighborhoods[2].size() == 3);
        CHECK(neighborhoods[3].size() == 1);
        CHECK(neighborhoods[4].size() == 2);
        CHECK(neighborhoods[5].size() == 1);
        CHECK(neighborhoods[6].size() == 0);
    };
}
