#include "xmldoc.h"

#include "xml.h"
#include "xslt.h"
#include "xsltstylesheet.h"

// xmlReadFd
#include <libxml/parser.h>

// xmlDoc
// xmlDocPtr
// xmlFreeDoc
// xmlSaveFormatFileEnc
#include <libxml/tree.h>

// xsltApplyStylesheet
#include <libxslt/transform.h>

// std::runtime_error
#include <stdexcept>

namespace matxin {
xmlDoc::xmlDoc(int fd, const char *URL, const char *encoding, int options)
    : doc_(xmlReadFd(fd, URL, encoding, options), [](const xmlDocPtr doc) {
        XML::get();
        xmlFreeDoc(doc);
      }) {
  XML::get();

  if (doc_.get() == NULL)
    throw std::runtime_error("");
}

xmlDoc::xmlDoc(const xsltStylesheet &style, const xmlDoc &doc,
               const char **params)
    : doc_(xsltApplyStylesheet(style.get_style(), doc.get_doc(), params),
           [](const xmlDocPtr doc) {
             XML::get();
             xmlFreeDoc(doc);
           }) {
  XSLT::get();

  if (doc_.get() == NULL)
    throw std::runtime_error("");
}

const xmlDocPtr xmlDoc::get_doc() const { return doc_.get(); }

int xmlSaveFormatFileEnc(const char *filename, xmlDoc cur, const char *encoding,
                         int format) {
  XML::get();
  return ::xmlSaveFormatFileEnc(filename, cur.get_doc(), encoding, format);
}
}
