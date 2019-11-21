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

#include "../../libs/utility/fs/directory_listing.hpp"


namespace fs = ::std::filesystem;
namespace po = ::boost::program_options;


template<typename CharT>
void print_tree(const utility::fs::basic_directory_listing<CharT>& directoryTree, std::uint32_t depth = 0) {
    std::cout << std::string(depth, ' ') << directoryTree.path << std::endl;
    for (const auto& child : directoryTree.children) {
        print_tree(child, depth + 1);
    }
}


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

    auto path = std::string(".");
    auto listing = utility::fs::list(path, 2, fs::directory_options::none);
    std::cout << "-------------------------------------" << std::endl;
    if (listing)
        print_tree(*listing);

    std::cout << "Success" << std::endl;
    return 0;
}













