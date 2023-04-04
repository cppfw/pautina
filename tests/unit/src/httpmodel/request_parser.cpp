#include <tst/set.hpp>
#include <tst/check.hpp>

#include <pautina/httpmodel/request_parser.hpp>

namespace{
std::string print_parsed_http_request(const pautina::http::request& r){
    std::stringstream ss;

    ss << "method = " << pautina::httpmodel::method_to_string(r.method) << std::endl;
    ss << "path = " << r.path << std::endl;
    ss << "protocol = " << pautina::httpmodel::protocol_to_string(r.protocol) << std::endl;

    for(const auto& h : r.headers.get_map()){
        ss << "name: " << h.first << std::endl;
        ss << "value: " << h.second << std::endl;
    }
    
    return ss.str();
}
}

namespace{
tst::set set("httpmodel__request_parser", [](tst::suite& suite){
    suite.add<std::pair<std::string, std::string>>(
        "samples",
        {
            {
                "GET / HTTP/1.0" "\n"
                "\n",

                "method = GET" "\n"
                "path = /" "\n"
                "protocol = HTTP/1.0" "\n"
            },
            {
                "GET / HTTP/1.1" "\n"
                "Host: localhost:8080" "\n"
                "User-Agent: curl/7.74.0" "\n"
                "Accept: */*" "\n"
                "\n",

                "method = GET" "\n"
                "path = /" "\n"
                "protocol = HTTP/1.1" "\n"
                "name: Accept" "\n"
                "value: */*" "\n"
                "name: Host" "\n"
                "value: localhost:8080" "\n"
                "name: User-Agent" "\n"
                "value: curl/7.74.0" "\n"
            },
            {
                "POST / HTTP/1.1" "\n"
                "Host: localhost:8080" "\n"
                "User-Agent: curl/7.74.0" "\n"
                "Accept: */*" "\n"
                "Content-Length: 11" "\n"
                "Content-Type: application/x-www-form-urlencoded" "\n"
                "\n"
                "[post data]",

                "method = POST" "\n"
                "path = /" "\n"
                "protocol = HTTP/1.1" "\n"
                "name: Accept" "\n"
                "value: */*" "\n"
                "name: Content-Length" "\n"
                "value: 11" "\n"
                "name: Content-Type" "\n"
                "value: application/x-www-form-urlencoded" "\n"
                "name: Host" "\n"
                "value: localhost:8080" "\n"
                "name: User-Agent" "\n"
                "value: curl/7.74.0" "\n"
            }
        },
        [](const auto& p){
            pautina::http::request_parser parser;

            parser.feed(utki::make_span<const uint8_t>(
                reinterpret_cast<const uint8_t*>(p.first.data()),
                p.first.size()
            ));

            tst::check(parser.is_end(), SL) << "request = \n" << p.first;

            auto res = print_parsed_http_request(parser.request);

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
            "GET / HTTP/1.1" "\n" // NOLINT(bugprone-suspicious-missing-comma)
                "Ho",
            "GET / HTTP/1.1" "\n"
                "Host:",
            "GET / HTTP/1.1" "\n"
                "Host: ",
            "GET / HTTP/1.1" "\n"
                "Host: localhost:8080" "\n"
                "User-Agent: curl/",
            "GET / HTTP/1.1" "\n"
                "Host: localhost:8080" "\n"
                "User-Agent: curl/7.74.0" "\n"
                "Accept: */*" "\n",
        },
        [](const auto& p){
            pautina::http::request_parser parser;

            parser.feed(
                utki::make_span(
                    reinterpret_cast<const uint8_t*>(p.data()),
                    p.size()
                )
            );

            tst::check(!parser.is_end(), SL);
        }
    );
});
}
