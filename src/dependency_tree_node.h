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
class DependencyTreeNode {
public:
  DependencyTreeNode(std::wstring &&line);

  template <class StringType>
  static std::vector<StringType> &&
  split(StringType &&string_, typename StringType::value_type delimiter);

private:
  long id_;
  std::experimental::optional<std::wstring> form_;
  Word word_;
  long head_;
  Relation deprel_;
};
}

#endif // DEPENDENCY_TREE_NODE_H
