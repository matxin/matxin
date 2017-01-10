#ifndef DEPENDENCY_TREE_NODE_H
#define DEPENDENCY_TREE_NODE_H

// std::wstring
#include <string>

// std::vector
#include <vector>

// std::experimental::optional
#include <experimental/optional>

namespace matxin {
class DependencyTreeNode {
public:
  DependencyTreeNode(std::wstring &&line);

private:
  template <class StringType>
  static std::vector<StringType> &&
  split(StringType &&string_, typename StringType::value_type delimiter);

  long id_;
  std::experimental::optional<std::wstring> form_;
  long head_;
};
}

#endif // DEPENDENCY_TREE_NODE_H
