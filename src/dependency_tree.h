#ifndef DEPENDENCY_TREE_H
#define DEPENDENCY_TREE_H

// std::wistream
#include <istream>

// std::vector
#include <vector>

namespace matxin {
class DependencyTree {
public:
  DependencyTree(std::wistream &conll_u);
};
}

#endif // DEPENDENCY_TREE_H
