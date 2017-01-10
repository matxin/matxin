#include "dependency_tree_node.h"

// std::runtime_error
#include <stdexcept>

// std::stoul
// std::wstring
#include <string>

// std::move
#include <utility>

// std::vector
#include <vector>

namespace matxin {
DependencyTreeNode::DependencyTreeNode(std::wstring &&line) {
  auto fields(split(std::move(line), L'\t'));

  if (fields.size() != 10)
    throw std::runtime_error("");
}

template <class StringType>
std::vector<StringType> &&
DependencyTreeNode::split(StringType &&line,
                          typename StringType::value_type delimiter) {
  std::vector<StringType> fields;

  for (typename StringType::size_type delimiter_search_start = 0;;) {
    typename StringType::size_type delimiter_index =
        line.find_first_of(delimiter, delimiter_search_start);
    fields.push_back(
        std::move(line.substring(delimiter_search_start, delimiter_index)));

    if (delimiter_index == StringType::npos)
      return std::move(fields);

    delimiter_search_start = delimiter_index + 1;
  }
}
}
