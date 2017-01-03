#ifndef XSLT_H
#define XSLT_H

namespace matxin {
class XSLT {
public:
  static const XSLT &get() {
    static XSLT xslt;
    return xslt;
  }

  ~XSLT();

  XSLT(const XSLT &xslt) = delete;
  XSLT(XSLT &&xslt) = delete;
  XSLT &operator=(const XSLT &xslt) = delete;
  XSLT &operator=(XSLT &&xslt) = delete;

private:
  XSLT() = default;
};
}

#endif // XSLT_H
