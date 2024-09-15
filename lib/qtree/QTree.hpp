#ifndef QTREE_HPP
#define QTREE_HPP

#include "qtree/QNode.hpp"

class QTree {
    QNode root;
    double size;
    umap<size_t, dd> positions;

   public:
    QTree(double size, uint maxEntriesPerNode);
    void insert(double x, double y, size_t index);
    // umap<size_t, uset<size_t>> getNeighborhoods(double r) const;

    size_t getNumEntries() const;
};

#endif  // QTREE_HPP
