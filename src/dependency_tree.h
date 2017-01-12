#ifndef DEPENDENCY_TREE_H
#define DEPENDENCY_TREE_H

#include "dependency_tree_node.h"

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
  DependencyTree(std::wstring &line, std::wistream &conll_u);

protected:
  friend DependencyTreeNode;
  std::map<long, DependencyTreeNode> nodes_;

  const decltype(nodes_) &get_nodes() const { return nodes_; }
  decltype(nodes_) &get_nodes() {
    return const_cast<decltype(nodes_) &>(
        static_cast<const decltype(this)>(this)->get_nodes());
  }

private:
  void get_not_empty_line(std::wstring &line, std::wistream &conll_u);
};
}

#endif // DEPENDENCY_TREE_H
