#include "xsltstylesheet.h"

#include "xml.h"
#include "xmldoc.h"
#include "xslt.h"

// xmlReadMemory
#include <libxml/parser.h>

// xmlDocPtr
// xmlFreeDoc
#include <libxml/tree.h>

// xsltFreeStylesheet
// xsltParseStylesheetDoc
// xsltStylesheet
// xsltStylesheetPtr
#include <libxslt/xsltInternals.h>

// std::function
#include <functional>

// std::shared_ptr
// std::unique_ptr
#include <memory>

// std::runtime_error
#include <stdexcept>

namespace matxin {
xsltStylesheet::xsltStylesheet(const char *buffer, int size, const char *URL,
                               const char *encoding, int options) {
  XML::get();
  XSLT::get();
  std::unique_ptr< ::xmlDoc, std::function<void(const xmlDocPtr)>> doc(
      xmlReadMemory(buffer, size, URL, encoding, options),
      [](const xmlDocPtr doc) {
        XML::get();
        xmlFreeDoc(doc);
      });
  style_ = std::shared_ptr< ::xsltStylesheet>(xsltParseStylesheetDoc(doc.get()),
                                             [](const xsltStylesheetPtr style) {
                                               XSLT::get();
                                               xsltFreeStylesheet(style);
                                             });

  if (style_.get() == NULL)
    throw std::runtime_error("");

  doc.release();
}

const xsltStylesheetPtr xsltStylesheet::get_style() const {
  return style_.get();
}
}
