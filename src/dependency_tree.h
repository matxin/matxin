#ifndef DEPENDENCY_TREE_H
#define DEPENDENCY_TREE_H

// std::wistream
#include <istream>

// std::map
#include <map>

// std::vector
#include <vector>

namespace matxin {
class DependencyTreeNode;

class DependencyTree {
public:
  DependencyTree(std::wistream &conll_u);

private:
  std::map<long, DependencyTreeNode> nodes_;
};
}

#endif // DEPENDENCY_TREE_H
