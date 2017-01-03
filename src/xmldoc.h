#ifndef XMLDOC_H
#define XMLDOC_H

// xmlDoc
// xmlDocPtr
#include <libxml/tree.h>

#include <memory>

namespace matxin {
class xsltStylesheet;

class xmlDoc {
public:
  // xmlReadFd
  xmlDoc(int fd, const char *URL, const char *encoding, int options);

  // xmlReadMemory
  xmlDoc(const char *buffer, int size, const char *URL, const char *encoding,
         int options);

  // xsltApplyStylesheet
  xmlDoc(const xsltStylesheet &style, const xmlDoc &doc, const char **params);

private:
  friend xsltStylesheet;

  const xmlDocPtr get_doc() const;

  std::shared_ptr<::xmlDoc> doc_;
};
}

#endif // XMLDOC_H
