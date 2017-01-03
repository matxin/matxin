#ifndef XMLDOC_H
#define XMLDOC_H

// xmlDocPtr
#include <libxml/tree.h>

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

  ~xmlDoc();

  xmlDoc(const xmlDoc &doc) = delete;
  xmlDoc(xmlDoc &&doc) = delete;
  xmlDoc &operator=(const xmlDoc &doc) = delete;
  xmlDoc &operator=(xmlDoc &&doc) = delete;

private:
  friend xsltStylesheet;
  const xmlDocPtr &get_doc() const { return doc_; }

  const xmlDocPtr doc_;
};
}

#endif // XMLDOC_H
