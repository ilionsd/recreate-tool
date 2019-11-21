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
#include <string_view>
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
        const std::filesystem::directory_options& options) -> basic_directory_listing<CharT> {
    auto path = generic_path_as<CharT>(directory.path());
    auto node = basic_directory_listing<CharT>(path);
    //std::cout << depth << std::endl;
    if (depth) {
        for (const auto& entry : std::filesystem::directory_iterator(directory, options)) {
            auto doFollowSymlink = static_cast<bool>(std::filesystem::directory_options::follow_directory_symlink & options);
            if (entry.is_directory() && (doFollowSymlink || !entry.is_symlink())) {
                node.children.push_back(list_impl<CharT>(entry, depth - 1, options));
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
        const std::filesystem::directory_options& options) {
    std::error_code ec;
    std::filesystem::directory_entry directory(path, ec);
    return (ec) ? std::optional<basic_directory_listing<CharT>>() : std::optional(detail::list_impl<CharT>(directory, depth, options));
}

}   //-- namespace utility::fs --


#endif /* UTILITY_FS_DIRECTORY_LISTING_HPP_ */
