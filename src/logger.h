#ifndef LOGGER_H
#define LOGGER_H

#ifdef NDEBUG
#define LOG(...)
#define LOGN(...)
#else
#include <cstring>
#include <iostream>

#define _LOG_INFO_FILE     //<< ", file: " << __FILE__
#define _LOG_INFO_FUNCTION //<< "    " << __FUNCTION__
#define _LOG_INFO_LINE     //<< "  line: " << __LINE__
#define DLOG(...)           \
    std::cout << "LOG: ";   \
    std::cout, __VA_ARGS__; \
    std::cout _LOG_INFO_FUNCTION _LOG_INFO_LINE _LOG_INFO_FILE << std::endl;

// Debug LOG with variable Name
#define DLOGN(...)                              \
    std::cout << "LOGN: ";                      \
    show(std::cout, #__VA_ARGS__, __VA_ARGS__); \
    std::cout _LOG_INFO_FUNCTION _LOG_INFO_LINE _LOG_INFO_FILE << std::endl;

template <typename T>
std::ostream &operator,(std::ostream &out, const T &t) {
    out << "  " << t;
    return out;
}

template <typename H1>
std::ostream &show(std::ostream &out, const char *label, H1 &&value) {
    return out << label << " = " << std::forward<H1>(value);
}

template <typename H1, typename... T>
std::ostream &show(std::ostream &out, const char *label, H1 &&value, T &&... rest) {
    const char *pcomma = strchr(label, ',');
    return show(out.write(label, pcomma - label) << " = " << std::forward<H1>(value) << ',',
                pcomma + 1, std::forward<T>(rest)...);
}
#endif
#endif // LOGGER_H
