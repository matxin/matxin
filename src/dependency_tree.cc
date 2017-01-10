#include "dependency_tree.h"

#include "dependency_tree_node.h"

// std::wistream
#include <istream>

// std::wstring
#include <string>

namespace matxin {
DependencyTree::DependencyTree(std::wistream &conll_u) {
  while (true) {
    std::wstring line;
    std::getline(conll_u, line);

    // The last line should be empty (but have a trailing newline), so only on
    // its next call will getline encounter the end-of-file.
    if (!conll_u)
      break;

    // ``Blank lines marking sentence boundaries''
    if (line.empty()) {
    }

    // ``Comment lines starting with hash (#)''
    if (line[0] == L'#')
      continue;

    try {
      DependencyTreeNode node(std::move(line));
      nodes_.emplace(node.get_id(), std::move(node));
    } catch (const std::runtime_error &exception) {
      throw;
    }
  }
}
}
