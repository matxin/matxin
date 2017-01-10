#ifndef DEPENDENCY_TREE_NODE_H
#define DEPENDENCY_TREE_NODE_H

// std::wstring
#include <string>

// std::vector
#include <vector>

// std::experimental::optional
#include <experimental/optional>

#include "relation.h"
#include "word.h"

namespace matxin {
class DependencyTree;

class DependencyTreeNode {
public:
  template <class StringType>
  static std::vector<StringType> &&
  split(StringType &&string_, typename StringType::value_type delimiter);

  DependencyTreeNode(std::wstring &&line);

  long get_id() { return id_; }

  void link(DependencyTree &tree);

private:
  long id_;
  std::experimental::optional<std::wstring> form_;
  Word word_;
  long head_;
  Relation deprel_;
  std::vector<DependencyTreeNode*> dependents_;
};
}

#endif // DEPENDENCY_TREE_NODE_H
