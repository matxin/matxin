#include "word.h"

#include "feats.h"
#include "upostag.h"

// std::wstring
#include <string>

// std::move
#include <utility>

namespace matxin {
Word::Word(std::wstring &&lemma, Upostag &&upostag, Feats &&feats)
    : lemma_(std::move(lemma)), upostag_(std::move(upostag)),
      feats_(std::move(feats)) {}
}
