#include "xsltstylesheet.h"

#include "xmldoc.h"

// xsltFreeStylesheet
// xsltParseStylesheetDoc
#include <libxslt/xsltInternals.h>

namespace matxin {
xsltStylesheet::xsltStylesheet(const xmlDoc &doc)
    : style_(xsltParseStylesheetDoc(doc.get_doc())) {
  if (style_ == NULL)
    throw;
}

xsltStylesheet::~xsltStylesheet() { xsltFreeStylesheet(style_); }
}
