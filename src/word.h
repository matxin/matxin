#ifndef WORD_H
#define WORD_H

// std::wstring
#include <string>

#include "feats.h"
#include "upostag.h"

namespace matxin {
class Word {
public:
  Word() = default;
  Word(std::wstring &&lemma, Upostag &&Upostag, Feats &&feats);

private:
  std::wstring lemma_;
  Upostag upostag_;
  Feats feats_;
};
}

#endif // WORD_H
