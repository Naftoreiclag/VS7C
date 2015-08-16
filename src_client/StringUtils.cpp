#include "StringUtils.h"

namespace skm {

namespace StringUtils {

const wchar_t* toWideString(std::string value) {
    std::wstring ws(value.begin(), value.end());
    return ws.c_str();
}

}

}
