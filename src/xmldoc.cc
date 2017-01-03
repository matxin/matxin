#include "xmldoc.h"

#include "xsltstylesheet.h"

// xmlReadFd
// xmlReadMemory
#include <libxml/parser.h>

// xmlDoc
// xmlDocPtr
// xmlFreeDoc
// xmlSaveFormatFileEnc
#include <libxml/tree.h>

// xsltApplyStylesheet
#include <libxslt/transform.h>

namespace matxin {
xmlDoc::xmlDoc(int fd, const char *URL, const char *encoding, int options)
    : doc_(xmlReadFd(fd, URL, encoding, options),
           [](const xmlDocPtr doc) { xmlFreeDoc(doc); }) {
  if (doc_.get() == NULL)
    throw;
}

xmlDoc::xmlDoc(const char *buffer, int size, const char *URL,
               const char *encoding, int options)
    : doc_(xmlReadMemory(buffer, size, URL, encoding, options),
           [](const xmlDocPtr doc) { xmlFreeDoc(doc); }) {
  if (doc_.get() == NULL)
    throw;
}

xmlDoc::xmlDoc(const xsltStylesheet &style, const xmlDoc &doc,
               const char **params)
    : doc_(xsltApplyStylesheet(style.get_style(), doc.get_doc(), params),
           [](const xmlDocPtr doc) { xmlFreeDoc(doc); }) {
  if (doc_.get() == NULL)
    throw;
}

const xmlDocPtr xmlDoc::get_doc() const { return doc_.get(); }

int xmlSaveFormatFileEnc(const char *filename, xmlDoc cur, const char *encoding,
                         int format) {
  ::xmlSaveFormatFileEnc(filename, cur.get_doc(), encoding, format);
}
}