#ifndef UPOSTAG_H
#define UPOSTAG_H

// std::wstring
#include <string>

namespace matxin {
class Upostag {
public:
  Upostag() = default;
  Upostag(std::wstring &&upostag_);

private:
  std::wstring upostag_;
};
}

#endif // UPOSTAG_H
