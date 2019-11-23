#include <climits>
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

#include <json11/json11.hpp>

#include "../utility/fs/directory_listing.hpp"


#define VERSION "Come on! I'm trying!'"


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
    po::options_description general("General");
    general.add_options()
            ("help"   , "produce help message")
            ("version", "produce program version");

    po::options_description options("Options");
    options.add_options()
            ("depth", po::value<std::uint32_t>()->default_value(std::numeric_limits<std::uint32_t>::max()), "depth of the listing/restoring")
            ("path" , po::value<std::string>()->default_value("."), "target directory");

    po::options_description description("Allowed options");
    description.add(general).add(options);

    po::positional_options_description pod;
    pod.add("path", -1);

    auto parsed = po::command_line_parser(argc, argv).options(description).positional(pod).run();

    po::variables_map vm;
    po::store(parsed, vm);
    po::notify(vm);

    if (!vm.size()) {
        std::cout << description << std::endl;
    }
    else if (vm.count("help")) {
        std::cout << description << std::endl;
    }
    else if (vm.count("version")) {
        std::cout << VERSION << std::endl;
    }

    auto depth = vm["depth"].as<std::uint32_t>();
    auto path = vm["path"].as<std::string>();

    if (!std::cin.eof()) {
        //-- recreating --
    }
    else {
        //-- listing --
        auto listing = utility::fs::list(path, depth, fs::directory_options::skip_permission_denied);
        json11::Json json = json11::Json::object{};
    }

    std::cout << "Success" << std::endl;
    return 0;
}

template<typename CharT>
auto listing_to_json(const ::utility::fs::basic_directory_listing<CharT>& listing) {
    using json11::Json;
    Json::array array;

    for (const auto& entry : listing.children) {
        array.push_back(listing_to_json<CharT>(entry));
    }

    return Json::object {
        { "path"    , listing.path },
        { "children", array }
    };
}













