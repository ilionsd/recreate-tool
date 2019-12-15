/*
 * directory_listing.hpp
 *
 *  Created on: Nov 21, 2019
 *      Author: ilion
 */

#ifndef UTILITY_FS_DIRECTORY_LISTING_HPP_
#define UTILITY_FS_DIRECTORY_LISTING_HPP_


#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "directory_path.hpp"


namespace utility::fs {


template<typename CharT>
struct basic_directory_listing {
    using char_type = CharT;
    using string_type = std::basic_string<char_type>;
    using self_type = basic_directory_listing<char_type>;

    basic_directory_listing(const string_type& str)
        : path(str)
        , children()
    {}

    string_type path;
    std::vector<self_type> children;
};


namespace detail {

template<typename CharT>
auto list_impl(
        const std::filesystem::directory_entry& directory,
        const std::uint32_t depth,
        const bool doFollowSymlinks) -> basic_directory_listing<CharT> {
    using ::std::filesystem::directory_iterator;
    using ::std::filesystem::directory_options;
    auto path = generic_path_as<CharT>(directory.path());
    auto node = basic_directory_listing<CharT>(path);
    if (depth) {
        for (const auto& entry : directory_iterator(directory, directory_options::skip_permission_denied)) {
            if (entry.is_directory() && (doFollowSymlinks || !entry.is_symlink())) {
                node.children.push_back(list_impl<CharT>(entry, depth - 1, doFollowSymlinks));
            }
        }
    }
    return node;
}

}   //-- namespace detail --

template<typename CharT>
auto list(
        const std::basic_string<CharT>& path,
        const std::uint32_t depth,
        const bool doFollowSymlinks = false) {
    using ::std::filesystem::directory_entry;
    std::error_code ec;
    directory_entry directory(path, ec);
    return (ec) ? std::optional<basic_directory_listing<CharT>>()
        : std::optional(detail::list_impl<CharT>(directory, depth, doFollowSymlinks));
}

}   //-- namespace utility::fs --


#endif /* UTILITY_FS_DIRECTORY_LISTING_HPP_ */
