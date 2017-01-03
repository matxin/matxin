#ifndef XSLTSTYLESHEET_H
#define XSLTSTYLESHEET_H

// xsltStylesheetPtr
#include <libxslt/xsltInternals.h>

namespace matxin {
class xmlDoc;

class xsltStylesheet {
public:
  xsltStylesheet(const xmlDoc &doc);

  ~xsltStylesheet();

  xsltStylesheet(const xsltStylesheet &style) = delete;
  xsltStylesheet(xsltStylesheet &&style) = delete;
  xsltStylesheet &operator=(const xsltStylesheet &style) = delete;
  xsltStylesheet &operator=(xsltStylesheet &&style) = delete;

private:
  friend xmlDoc;
  const xsltStylesheetPtr &get_style() const { return style_; }

  const xsltStylesheetPtr style_;
};
}

#endif // XSLTSTYLESHEET_H
