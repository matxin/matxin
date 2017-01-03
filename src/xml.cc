#include "xml.h"

// xmlCleanupParser
#include <libxml/parser.h>

namespace matxin {
XML::~XML() { xmlCleanupParser(); }
}
