#include "xmldoc.h"

#include "xsltstylesheet.h"

// xmlReadFd
// xmlReadMemory
#include <libxml/parser.h>

// xmlDoc
// xmlDocPtr
// xmlFreeDoc
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
}
