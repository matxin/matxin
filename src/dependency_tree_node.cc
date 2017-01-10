#include "dependency_tree_node.h"

#include "dependency_tree.h"

// std::runtime_error
#include <stdexcept>

// std::stoul
// std::wstring
#include <string>

// std::move
#include <utility>

// std::vector
#include <vector>

#include "feats.h"
#include "relation.h"
#include "upostag.h"

namespace matxin {
template <class StringType>
std::vector<StringType> &&
DependencyTreeNode::split(StringType &&string_,
                          typename StringType::value_type delimiter) {
  std::vector<StringType> string_vector;

  for (typename StringType::size_type delimiter_search_start = 0;;) {
    typename StringType::size_type delimiter_index =
        string_.find_first_of(delimiter, delimiter_search_start);
    string_vector.push_back(
        std::move(string_.substring(delimiter_search_start, delimiter_index)));

    if (delimiter_index == StringType::npos)
      return std::move(string_vector);

    delimiter_search_start = delimiter_index + 1;
  }
}

DependencyTreeNode::DependencyTreeNode(std::wstring &&line) {
  auto fields(split(std::move(line), L'\t'));

  if (fields.size() != 10)
    throw std::runtime_error("");

  id_ = std::stoul(fields[0]);
  form_ = std::move(fields[1]);
  word_ = std::move(Word(std::move(fields[2]),
                         std::move(Upostag(std::move(fields[3]))),
                         std::move(Feats(std::move(fields[5])))));
  head_ = std::stoul(fields[6]);
  deprel_ = std::move(Relation(std::move(fields[7])));
}

void DependencyTreeNode::link(DependencyTree &tree) {
  tree.get_nodes().at(id_)->dependents_.push_back(this);
}
}
