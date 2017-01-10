#include "relation.h"

// std::wstring
#include <string>

namespace matxin {
Relation::Relation(std::wstring &&deprel) : deprel_(deprel) {}
}
