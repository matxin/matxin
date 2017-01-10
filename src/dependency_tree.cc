#include "dependency_tree.h"

// std::wistream
#include <istream>

// std::wstring
#include <string>

// std::move
#include <utility>

// std::vector
#include <vector>

namespace matxin {
DependencyTree::DependencyTree(std::wistream conll_u) {
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

    std::vector<std::wstring> word_line_fields = split(line, L'\t');
  }
}

template <class StringType>
std::vector<StringType> &&
DependencyTree::split(StringType string_,
                      typename StringType::value_type delimiter) {
  std::vector<StringType> fields;

  for (typename StringType::size_type delimiter_search_start = 0;;) {
    typename StringType::size_type delimiter_index =
        string_.find_first_of(delimiter, delimiter_search_start);
    fields.push_back(
        std::move(string_.substring(delimiter_search_start, delimiter_index)));

    if (delimiter_index == StringType::npos)
      return std::move(fields);

    delimiter_search_start = delimiter_index + 1;
  }
}
}
