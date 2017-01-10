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

protected:
  friend DependencyTreeNode;
  std::map<long, DependencyTreeNode*> nodes_;

  decltype(nodes_) get_nodes() { return nodes_; }
};
}

#endif // DEPENDENCY_TREE_H
