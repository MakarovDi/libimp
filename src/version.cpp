#include "imp/version"
#include <ex/macro>


#ifdef HAVE_CONFIG_H
#  include "imp/config"
#else
#  define VERSION_MAJOR    0
#  define VERSION_MINOR    1
#  define VERSION_REVISION 0
#endif


namespace imp
{
    const uint8_t version::kMajor    = VERSION_MAJOR;
    const uint8_t version::kMinor    = VERSION_MINOR;
    const uint8_t version::kRevision = VERSION_REVISION;
    const char*   version::kString   = "v" LITERAL(VERSION_MAJOR) "." LITERAL(VERSION_MINOR) "." LITERAL(VERSION_REVISION);
}

