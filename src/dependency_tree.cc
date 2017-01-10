#include "dependency_tree.h"

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
    // its next call with getline encounter the end-of-file.
    if (!conll_u)
      break;

    // ``Blank lines marking sentence boundaries''
    if (line.empty()) {
    }

    // ``Comment lines starting with hash (#)''
    if (line[0] == L'#')
      continue;
  }
}
}
