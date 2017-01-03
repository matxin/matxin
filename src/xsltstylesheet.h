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
  xsltStylesheet(const xmlDoc &doc);

private:
  friend xmlDoc;

  const xsltStylesheetPtr get_style() const;

  std::shared_ptr<::xsltStylesheet> style_;
};
}

#endif // XSLTSTYLESHEET_H
