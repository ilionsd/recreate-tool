#include <climits>
#include <cstddef>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include <boost/program_options.hpp>
#include <json11/json11.hpp>
#include "../utility/fs/directory_listing.hpp"


#define VERSION "Come on! I'm trying!'"


using json11::Json;

template<typename CharT>
auto empty(const std::basic_istream<CharT>& ) -> bool;

template<typename CharT>
auto read_stream(std::basic_istream<CharT>& ) -> std::basic_string<CharT>;

template<typename CharT>
auto listing_to_json(const ::utility::fs::basic_directory_listing<CharT>& ) -> Json;

template<typename CharT>
auto json_to_listing(const Json& ) -> ::utility::fs::basic_directory_listing<CharT>;


int main(int argc, char **argv) {
    using std::cin;
    using std::cout;
    using std::cerr;
    using std::endl;
    using std::uint32_t;
    using std::string;
    namespace po = ::boost::program_options;


    po::options_description general("General");
    general.add_options()
            ("help"   , "produce help message")
            ("version", "produce program version");

    po::options_description options("Options");
    options.add_options()
            ("depth", po::value<uint32_t>()->default_value(std::numeric_limits<uint32_t>::max()), "depth of the listing/restoring")
            ("path" , po::value<string>()->default_value("."), "target directory");

    po::options_description description("Allowed options");
    description.add(general).add(options);

    po::positional_options_description pod;
    pod.add("path", -1);

    auto parsed = po::command_line_parser(argc, argv).options(description).positional(pod).run();

    po::variables_map vm;
    po::store(parsed, vm);
    po::notify(vm);

    if (!vm.size()) {
        cout << description << endl;
    }
    else if (vm.count("help")) {
        cout << description << endl;
    }
    else if (vm.count("version")) {
        cout << VERSION << endl;
    }

    auto depth = vm["depth"].as<uint32_t>();
    auto path = vm["path"].as<string>();

    if (empty(std::cin)) {
        //-- listing --
        auto oListing = ::utility::fs::list(path, depth, false);
        if (!oListing) {
            cerr << "Failed to list path: " << path << endl;
        }
        else {
            auto json = listing_to_json(*oListing);
            cout << json.dump();
        }
    }
    else {
        //-- recreating --
        std::string in = read_stream(std::cin);
        std::string err;
        auto json = Json::parse(in, err);
    }

    return 0;
}

template<typename CharT>
auto empty(const std::basic_istream<CharT>& is) -> bool {
    return !(is.rdbuf()-> in_avail() > 0);
}

template<typename CharT>
auto read_stream(std::basic_istream<CharT>& is) -> std::basic_string<CharT> {
    std::basic_stringstream<CharT> ss;
    for (std::string line; getline(is, line); ) {
        ss << line;
    }
    return ss.str();
}

template<typename CharT>
auto listing_to_json(const ::utility::fs::basic_directory_listing<CharT>& listing) -> Json {
    Json::array array;
    for (const auto& entry : listing.children) {
        array.push_back(listing_to_json<CharT>(entry));
    }
    return Json::object {
        { "path"    , listing.path },
        { "children", array }
    };
}

template<typename CharT>
auto json_to_listing(const Json& json) -> ::utility::fs::basic_directory_listing<CharT> {
    return {};
}











