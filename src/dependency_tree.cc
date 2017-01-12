#include "dependency_tree.h"

#include "dependency_tree_node.h"

// std::wistream
#include <istream>

// std::runtime_error
#include <stdexcept>

// std::wstring
#include <string>

namespace matxin {
DependencyTree::DependencyTree(std::wstring &line, std::wistream &conll_u) {
  // Ignore ``Comment lines starting with hash (#)''. A comment line is
  // expected only directly after an empty line and directly before a word
  // line.
  if (line[0] != L'#') {
    try {
      DependencyTreeNode node(line);
      nodes_.emplace(node.get_id(), std::move(node));
    } catch (const std::runtime_error &exception) {
      throw;
    }
  }

  get_not_empty_line(line, conll_u);

  for (;;) {
    // ``Blank lines marking sentence boundaries'' also mark the end of
    // construction.
    if (line.empty()) {
      {
        decltype(nodes_)::iterator nodes_end(nodes_.end());

        for (decltype(nodes_)::iterator nodes_iterator(nodes_.begin());
             nodes_iterator != nodes_end; ++nodes_iterator)
          nodes_iterator->second.link(this);
      }

      return;
    }

    get_not_empty_line(line, conll_u);
  }
}

void DependencyTree::get_not_empty_line(std::wstring &line,
                                        std::wistream &conll_u) {
  std::getline(conll_u, line);

  // Whether this is the object's first call of this method or any subsequent
  // call, end-of-file is unexpected.
  //
  // First call
  // ==========
  // EOF is expected only after an empty line, which is not expected yet. An
  // empty line is expected only directly after at least one word line.
  //
  // Any subsequent call
  // ===================
  // An empty line would have been handled earlier.
  if (!conll_u)
    throw std::runtime_error("Unexpected end-of-file before word line");
}
}
