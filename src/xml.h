#ifndef XML_H
#define XML_H

namespace matxin {
class XML {
public:
  static const XML &get() {
    static XML xml;
    return xml;
  }

  ~XML();

  XML(const XML &xml) = delete;
  XML(XML &&xml) = delete;
  XML &operator=(const XML &xml) = delete;
  XML &operator=(XML &&xml) = delete;

private:
  XML() = default;
};
}

#endif // XML_H
