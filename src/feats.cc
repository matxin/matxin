#include "feats.h"

#include "dependency_tree_node.h"

// std::runtime_error
#include <stdexcept>

// std::wstring
#include <string>

namespace matxin {
Feats::Feats(std::wstring &&feats) {
  if (feats == L"_")
    return;

  auto items(DependencyTreeNode::split(std::move(feats), L'|'));

  {
    decltype(items)::iterator items_end(std::move(items.end()));

    for (decltype(items)::iterator items_iterator(std::move(items.begin()));
         items_iterator != items_end; ++items_iterator) {
      auto item(DependencyTreeNode::split(std::move(*items_iterator), L'='));

      if (item.size() != 2)
        throw std::runtime_error("");

      feats_.emplace(std::move(item[0]), std::move(item[1]));
    }
  }
}
}
