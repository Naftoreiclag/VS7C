#include "Version.h"

#include <sstream>

namespace skm {

Version::Version(int32_t major, int32_t minor, int32_t patch)
: major(major)
, minor(minor)
, patch(patch) {
}

const std::string Version::str() const {
    std::stringstream ss;
    ss << major;
    ss << '.';
    ss << minor;
    ss << '.';
    ss << patch;

    return ss.str();
}

}
