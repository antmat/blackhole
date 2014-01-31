#pragma once

#include <cctype>
#include <string>

#include <boost/lexical_cast.hpp>

namespace blackhole {

namespace sink {

namespace rotation {

namespace matching {

template<typename Iterator>
inline bool parse_counter(Iterator& it, Iterator end, uint& width) {
    std::string value;
    while (it != end) {
        char c = *it;
        if (c == 'N') {
            width = value.empty() ? 1 : boost::lexical_cast<uint>(value);
            return true;
        } else if (std::isdigit(c)) {
            value.push_back(c);
        } else {
            return false;
        }

        it++;
    }

    return false;
}

inline bool parse_counter(const std::string& pattern, uint& width) {
    auto it = pattern.begin();
    return parse_counter(it, pattern.end(), width);
}

inline bool matched(const std::string& pattern, const std::string& filename) {
    auto f_it = filename.begin();
    auto f_end = filename.end();
    auto p_it = pattern.begin();
    auto p_end = pattern.end();

    bool placeholder_expected = false;
    while (f_it != filename.end() && p_it != pattern.end()) {
        char f_c = *f_it;
        char p_c = *p_it;

        if (!placeholder_expected) {
            if (p_c == '%') {
                placeholder_expected = true;
                p_it++;
            } else if (f_c == p_c) {
                f_it++;
                p_it++;
            } else {
                return false;
            }
        } else {
            switch (p_c) {
            case '%':
                if (p_c == f_c) {
                    ++p_it;
                    ++f_it;
                    break;
                } else {
                    return false;
                }
            case 'M':
            case 'H':
            case 'd':
            case 'm':
                if (!naming::all_digits(f_it, f_end, 2)) {
                    return false;
                }
                ++p_it;
                break;
            case 'Y':
                if (!naming::all_digits(f_it, f_end, 4)) {
                    return false;
                }
                ++p_it;
                break;
            default: {
                auto p = p_it;
                uint width = 0;
                if (!parse_counter(p, p_end, width)) {
                    throw std::invalid_argument("Unsupported placeholder used in pattern for file scanning");
                }

                auto f = f_it;
                if (!naming::all_digits(f, f_end, width)) {
                    return false;
                }

                while (f != f_end && std::isdigit(*f)) {
                    ++f;
                }

                try {
                    boost::lexical_cast<uint>(std::string(f_it, f));
                    if (std::distance(f_it, f) != width) {
                        return false;
                    }
                    f_it = f;
                } catch (const boost::bad_lexical_cast&) {
                    return false;
                }

                p_it = p + 1;
                }
                break;
            }

            placeholder_expected = false;
        }
    }

    if (p_it == p_end) {
        if (f_it != f_end) {
            return naming::all_digits(f_it, f_end, std::distance(f_it, f_end));
        } else {
            return true;
        }
    } else {
        return false;
    }
}

struct both_t {
    const std::string& pattern;

    both_t(const std::string& pattern) :
        pattern(pattern)
    {}

    bool operator ()(const std::string& filename) const {
        return !matched(pattern, filename);
    }
};

} // namespace match

} // namespace rotation

} // namespace sink

} // namespace blackhole