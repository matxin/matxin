#include "../dependency_tree.h"

// EXIT_FAILURE
#include <cstddef>

// std::exception
#include <exception>

// std::wcin
#include <iostream>

// std::getline
#include <string>

int main(int argc, char **argv) {
  try {
    std::wstring line;
    std::getline(std::wcin, line);

    if (!std::wcin) {
      std::cerr << argv[0] << ": Unexpected end-of-file\n";
      return EXIT_FAILURE;
    }

    matxin::DependencyTree dependency_tree(line, std::wcin);
  } catch (const std::exception &exception_) {
    std::cerr << argv[0] << ": " << exception_.what() << "\n";
    return EXIT_FAILURE;
  }
}
