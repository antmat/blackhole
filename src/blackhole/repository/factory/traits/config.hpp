#pragma once

#include <string>
#include <typeindex>

#include "blackhole/dynamic.hpp"

namespace blackhole {

template<typename T>
struct match_traits {
    //! \brief Extract sink or formatter type index from its config.
    /*! Some sinks or formatters require more complex string mapping to type,
     * than provided by its static method `name`.
     * For example, `files_t` sink can be configured for using rotation, and
     * that rotation can have various strategies to determine when and how
     * actually do it.
     * In that case we can define unique keys explicitly in configuration file
     * (files/rotation/size) or to build that key from config. I'd preferred
     * the second way, because it is more agile.
     * So, suppose we have config:
     * "files": {
     *     "path": "test.log",
     *     "rotation": {
     *         "pattern": "test.log.%N",
     *         "backups": 5,
     *         "size": 1000000
     *     }
     * }
     *
     * If we find `rotation` key, then rotation is used, so we just return
     * type index with specified rotation strategy.
     *
     * By default returns Formatter or Sink typeid;
     */
    static std::type_index ti(const std::string&, const dynamic_t&) {
        return typeid(T);
    }
};

} // namespace blackhole
