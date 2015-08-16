#ifndef VERSION_H
#define VERSION_H

#include <cstdint>
#include <string>

namespace skm {

struct Version {
    int32_t major = 0;
    int32_t minor = 0;
    int32_t patch = 0;

    Version(int32_t major, int32_t minor, int32_t patch);

    const std::string str() const;
};

}

#endif // VERSION_H
