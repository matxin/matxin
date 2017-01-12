#include "feats.h"

#include "dependency_tree_node.h"
#include "matxin_string_utils.h"

// std::runtime_error
#include <stdexcept>

// std::wstring
#include <string>

namespace matxin {
Feats::Feats(const std::wstring &feats) {
  if (feats == L"_")
    return;

  auto items(DependencyTreeNode::split(feats, L'|'));

  {
    decltype(items)::iterator items_end(items.end());

    for (decltype(items)::iterator items_iterator(items.begin());
         items_iterator != items_end; ++items_iterator) {
      auto item(DependencyTreeNode::split(*items_iterator, L'='));

      if (item.size() != 2)
        throw std::runtime_error(std::string("Can't split `") +
                                 wstos(*items_iterator) + "'");

      feats_.emplace(std::move(item[0]), std::move(item[1]));
    }
  }
}
}
