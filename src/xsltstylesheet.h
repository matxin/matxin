#ifndef XSLTSTYLESHEET_H
#define XSLTSTYLESHEET_H

// xsltStylesheet
// xsltStylesheetPtr
#include <libxslt/xsltInternals.h>

// std::shared_ptr
#include <memory>

namespace matxin {
class xmlDoc;

class xsltStylesheet {
public:
  // xmlReadMemory
  // xsltParseStylesheetDoc
  xsltStylesheet(const char *buffer, int size, const char *URL,
                 const char *encoding, int options);

private:
  friend xmlDoc;

  const xsltStylesheetPtr get_style() const;

  std::shared_ptr< ::xsltStylesheet> style_;
};
}

#endif // XSLTSTYLESHEET_H
