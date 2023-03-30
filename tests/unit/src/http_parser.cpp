#include <tst/set.hpp>
#include <tst/check.hpp>

#include <pautina/http_parser.hpp>

namespace{
std::string print_parsed_http_header(const pautina::http_parser& p){
    std::stringstream ss;

    ss << "method = " << pautina::http::method_to_string(p.method) << std::endl;
    ss << "path = " << p.path << std::endl;
    ss << "protocol = " << pautina::http::protocol_to_string(p.protocol) << std::endl;

    // TODO:
    return ss.str();
}
}

namespace{
tst::set set("http_parser", [](tst::suite& suite){
    suite.add<std::pair<std::string, std::string>>(
        "samples",
        {
            {
                "GET / HTTP/1.1\n",

                "method = GET" "\n"
                "path = /" "\n"
                "protocol = HTTP/1.1" "\n"
            }
        },
        [](const auto& p){
            pautina::http_parser parser;

            parser.feed(p.first);

            // TODO: uncomment
            // tst::check(parser.is_end(), SL);

            auto res = print_parsed_http_header(parser);

            tst::check_eq(res, p.second, SL);
        }
    );

    suite.add<std::string>(
        "unfinished_samples",
        {
            "GET",
            "GET /",
            "GET / ",
            "GET / HTT",
            "GET / HTTP/1.1",
            "GET / HTTP/1.1\n",
        },
        [](const auto& p){
            pautina::http_parser parser;

            parser.feed(p);

            tst::check(!parser.is_end(), SL);
        }
    );
});
}
