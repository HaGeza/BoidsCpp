#include <memory>

#include "typedefs.hpp"

class QNode {
    double x, y, size;
    size_t maxEntries, numEntries;
    vec<size_t> entryIndices;
    std::unique_ptr<QNode[]> children;

    void pickChild(size_t entryIndex, umap<size_t, dd>& positions);

   public:
    QNode() = default;
    QNode(double x, double y, double size, uint maxEntries);
    void insert(size_t entryIndex, umap<size_t, dd>& positions);
    // umap<size_t, uset<size_t>> getNeighborhoods(
    //     double r, umap<size_t, dd>& positions,
    //     vec<size_t> potentialNeighbors) const;

    size_t getNumEntries() const;
    const QNode* getChild(size_t index) const;
};

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
