#ifndef WORD_H
#define WORD_H

#include "feats.h"
#include "upostag.h"

// std::wstring
#include <string>

namespace matxin {
class Word {
public:
  Word() = default;
  Word(std::wstring &&lemma, Upostag &&upostag, Feats &&feats);

private:
  std::wstring lemma_;
  Upostag upostag_;
  Feats feats_;
};
}

#endif // WORD_H
