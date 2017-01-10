#ifndef DEPENDENCY_TREE_H
#define DEPENDENCY_TREE_H

// std::wistream
#include <istream>

// std::vector
#include <vector>

namespace matxin {
class DependencyTree {
public:
  DependencyTree(std::wistream conll_u);

private:
  template <class StringType>
  std::vector<StringType> &&split(StringType string_,
                                  typename StringType::value_type delimiter);
};
}

#endif // DEPENDENCY_TREE_H
