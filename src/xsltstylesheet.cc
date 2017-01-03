#include "xsltstylesheet.h"

#include "xmldoc.h"

// xsltFreeStylesheet
// xsltParseStylesheetDoc
// xsltStylesheetPtr
#include <libxslt/xsltInternals.h>

namespace matxin {
xsltStylesheet::xsltStylesheet(const xmlDoc &doc)
    : style_(xsltParseStylesheetDoc(doc.get_doc()),
             [](const xsltStylesheetPtr style) { xsltFreeStylesheet(style); }) {
  if (style_.get() == NULL)
    throw;
}

const xsltStylesheetPtr xsltStylesheet::get_style() const {
  return style_.get();
}
}
