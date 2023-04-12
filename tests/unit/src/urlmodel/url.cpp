#include <tst/set.hpp>
#include <tst/check.hpp>

#include <pautina/urlmodel/url.hpp>

namespace{
tst::set set("urlmodel__url", [](tst::suite& suite){
    suite.add<std::pair<
        std::vector<std::string>,
        std::vector<std::string>
    >>
    (
        "less__true",
        {
            {
                {},
                {"blah"}
            },
            {
                {"blah"},
                {"blah1"}
            },
            {
                {"blaa"},
                {"blah"}
            },
            {
                {"blah", "hi"},
                {"blah1", "hi"}
            },
            {
                {"blaa", "hi"},
                {"blah", "hi"}
            }
        },
        [](const auto& p){
            tst::check(urlmodel::less()(p.first, p.second), SL) << "\n"
                << "a = " << utki::make_span(p.first) << "\n"
                << "b = " << utki::make_span(p.second) << "\n";
        }
    );

    suite.add<std::pair<
        std::vector<std::string>,
        std::vector<std::string>
    >>
    (
        "less__false",
        {
            {
                {},
                {}
            },
            {
                {"blah"},
                {}
            },
            {
                {"blah"},
                {"blah"}
            },
            {
                {"blah1"},
                {"blah"}
            },
            {
                {"blah"},
                {"blaa"}
            },
            {
                {"blah1", "hi"},
                {"blah", "hi"}
            },
            {
                {"blah", "hi"},
                {"blaa", "hi"}
            }
        },
        [](const auto& p){
            tst::check(!urlmodel::less()(p.first, p.second), SL) << "\n"
                << "a = " << utki::make_span(p.first) << "\n"
                << "b = " << utki::make_span(p.second) << "\n";
        }
    );
});
}