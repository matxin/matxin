#ifndef FEATS_H
#define FEATS_H

// std::map
#include <map>

// std::wstring
#include <string>

namespace matxin {
class Feats {
public:
  Feats() = default;
  Feats(std::wstring &&feats);

private:
  std::map<std::wstring, std::wstring> feats_;
};
}

#endif // FEATS_H
