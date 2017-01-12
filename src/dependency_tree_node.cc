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

// DEBUG
#include "matxin_string_utils.h"
#include <iostream>

namespace matxin {
template <class StringType>
std::vector<StringType>
DependencyTreeNode::split(const StringType &string_,
                          typename StringType::value_type delimiter) {
  std::cerr << "splitting `" << wstos(string_) << "' on `"
            << wstos(StringType(1, delimiter)) << "'\n";
  std::vector<StringType> string_vector;

  for (typename StringType::size_type delimiter_search_start = 0;;) {
    typename StringType::size_type delimiter_index =
        string_.find_first_of(delimiter, delimiter_search_start);
    std::cerr << "matched `" << wstos(StringType(1, delimiter)) << "' at index "
              << delimiter_index << "\n";
    std::cerr << "split `"
              << wstos(string_.substr(delimiter_search_start,
                                      delimiter_index - delimiter_search_start))
              << "'\n";
    string_vector.emplace_back(string_.substr(
        delimiter_search_start, delimiter_index - delimiter_search_start));

    if (delimiter_index == StringType::npos)
      return string_vector;

    delimiter_search_start = delimiter_index + 1;
  }
}

DependencyTreeNode::DependencyTreeNode(const std::wstring &line) {
  auto fields(split(line, L'\t'));

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

void DependencyTreeNode::link(DependencyTree *const tree) {
  tree->get_nodes().at(id_).dependents_.push_back(this);
}
}
