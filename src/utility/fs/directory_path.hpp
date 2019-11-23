/*
 * directory_path.hpp
 *
 *  Created on: Nov 21, 2019
 *      Author: ilion
 */

#ifndef UTILITY_FS_DIRECTORY_PATH_HPP_
#define UTILITY_FS_DIRECTORY_PATH_HPP_


#include <filesystem>
#include <string>

namespace utility::fs {


template<typename CharT>
std::basic_string<CharT>
generic_path_as(const std::filesystem::path& path);

template<>
std::basic_string<char>
generic_path_as(const std::filesystem::path& path) { return path.generic_string(); }

template<>
std::basic_string<wchar_t>
generic_path_as(const std::filesystem::path& path) { return path.generic_wstring(); }

template<>
std::basic_string<char16_t>
generic_path_as(const std::filesystem::path& path) { return path.generic_u16string(); }

template<>
std::basic_string<char32_t>
generic_path_as(const std::filesystem::path& path) { return path.generic_u32string(); }


template<typename CharT>
std::basic_string<CharT>
native_path_as(const std::filesystem::path& path);

template<>
std::basic_string<char>
native_path_as(const std::filesystem::path& path) { return path.string(); }

template<>
std::basic_string<wchar_t>
native_path_as(const std::filesystem::path& path) { return path.wstring(); }

template<>
std::basic_string<char16_t>
native_path_as(const std::filesystem::path& path) { return path.u16string(); }

template<>
std::basic_string<char32_t>
native_path_as(const std::filesystem::path& path) { return path.u32string(); }


}   //-- namespace utility::fs --

#endif /* UTILITY_FS_DIRECTORY_PATH_HPP_ */
