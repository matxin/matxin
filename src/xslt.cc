#include "xslt.h"

// xsltCleanupGlobals
#include <libxslt/xslt.h>

namespace matxin {
XSLT::~XSLT() { xsltCleanupGlobals(); }
}
