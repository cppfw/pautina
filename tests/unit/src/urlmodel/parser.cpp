#include <tst/set.hpp>
#include <tst/check.hpp>

#include <pautina/urlmodel/parser.hpp>

namespace{
tst::set set("urlmodel__parser", [](tst::suite& suite){
    suite.add<std::pair<std::string, pautina::urlmodel::url>>(
        "samples",
        {
            {
                "http://user:password@host.com:8080/path/to/dir?param1=value1&param2=value2#fragment",
                pautina::urlmodel::url{
                    .scheme = "http",
                    .username = "user",
                    .password = "password",
                    .host = "host.com",
                    .port = 8080,
                    .path = {"path", "to", "dir"},
                    .query = {
                        {"param1", "value1"},
                        {"param2", "value2"}
                    },
                    .fragment = "fragment"
                }
            }
        },
        [](const auto& p){
            pautina::urlmodel::parser parser;

            // TODO: feed by several batches

            auto span = utki::make_span(
                reinterpret_cast<const uint8_t*>(p.first.data()),
                p.first.size()
            );

            parser.feed(span);

            tst::check(parser.url == p.second, SL)
                << "parsed = \n\t" << parser.url.to_string() << "\n"
                << "expected = \n\t" << p.second.to_string() << "\n";
        }
    );
});
}
