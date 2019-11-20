#include <cstddef>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <boost/program_options.hpp>


namespace fs = ::std::filesystem;
namespace po = ::boost::program_options;

template<typename CharT>
struct directory_tree {
    using char_type = CharT;
    using string_type = std::basic_string<char_type>;

    directory_tree(const string_type& str)
        : relative_path(str)
        , children()
    {}

    const string_type relative_path;
    std::vector<std::unique_ptr<directory_tree>> children;
};

template<typename CharT>
void print_tree(const directory_tree<CharT>& directoryTree, std::uint32_t depth = 0) {
    std::cout << std::string(depth, ' ') << directoryTree.relative_path << std::endl;
    for (const auto& child : directoryTree.children) {
        if (child) {
            print_tree(*child, depth + 1);
        }
    }
}

template<typename CharT>
auto listdir(const std::basic_string<CharT>& str) -> std::unique_ptr<directory_tree<CharT>>;

int main(int argc, char **argv) {
    // https://github.com/dropbox/json11/blob/master/LICENSE.txt
    /*
     --path
     --depth
     */
    po::options_description general("General");
    general.add_options()
            ("help", "produce help message")
            ("version", "produce program version");

    po::options_description options("Options");
    options.add_options()
            ("depth", po::value<std::uint32_t>(), "")
            ("path", po::value<std::string>(), "");

    po::options_description description("Allowed options");
    description.add(general).add(options);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);

    if (!vm.size()) {
        std::cout << description << std::endl;
    }
    if (vm.count("help")) {
        std::cout << description << std::endl;
    }

    using namespace std::string_literals;
    auto directoryTree = listdir(std::string("."));
    std::cout << "-------------------------------------" << std::endl;
    if (directoryTree)
        print_tree(*directoryTree);

    std::cout << "Success" << std::endl;
    return 0;
}

template<typename CharT>
auto listdir_impl(const fs::directory_entry& directory) -> std::unique_ptr<directory_tree<CharT>> {
    auto directoryTree = std::make_unique<directory_tree<CharT>>(directory.path().native());
    //std::cout << directory.path().native() << std::endl;
    for (const auto& dirIt : fs::directory_iterator(directory, fs::directory_options::skip_permission_denied)) {
        if (dirIt.is_directory() && !dirIt.is_symlink()) {
            directoryTree->children.emplace_back(std::move(listdir_impl<CharT>(dirIt)));
        }
    }
    return directoryTree;
}

template<typename CharT>
auto listdir(const std::basic_string<CharT>& str) -> std::unique_ptr<directory_tree<CharT>> {
    std::error_code ec;
    fs::path path(str);
    fs::directory_entry directory(path, ec);
    if (ec)
        return std::unique_ptr<directory_tree<CharT>>();
    else
        return listdir_impl<CharT>(directory);
}












