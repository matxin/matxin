#ifndef RELATION_H
#define RELATION_H

// std::wstring
#include <string>

namespace matxin {
class Relation {
public:
  Relation() = default;
  Relation(std::wstring &&deprel);

private:
  std::wstring deprel_;
};
}

#endif // RELATION_H
