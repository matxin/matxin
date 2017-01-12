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
  DependencyTreeNode *root_;
  std::map<long, DependencyTreeNode> nodes_;

private:
  void emplace_node(const std::wstring &line);
  void get_not_empty_line(std::wstring &line, std::wistream &conll_u);
};
}

#endif // DEPENDENCY_TREE_H
