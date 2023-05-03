#include <tst/set.hpp>
#include <tst/check.hpp>

#include <pautina/router.hpp>

namespace{
tst::set set("http__router", [](tst::suite& suite){
    suite.add<std::vector<std::string>>(
        "empty_route_list",
        {
            {},
            {"blah"},
            {"blah", "blah1"},
            {"blah", "blah1", "blah2"}
        },
        [](const auto& p){
            pautina::router r({});

            auto resp = r.route({
                .url = {
                    .path = p
                }
            });

            tst::check(resp.status == httpmodel::status::http_404_not_found, SL);
        }
    );

    suite.add<std::vector<std::string>>(
        "one_empty_route",
        {
            {},
            {"blah"},
            {"blah", "blah1"},
            {"blah", "blah1", "blah2"},
        },
        [](const auto& p){
            bool called = false;

            std::vector<std::string> the_subpath;

            pautina::router r(
                pautina::router::routes_type{
                    {
                        {},
                        [&](const auto& req, auto subpath){
                            called = true;
                            the_subpath = utki::make_vector(subpath);
                            return httpmodel::response(req, httpmodel::status::http_200_ok);
                        }
                    }
                }
            );

            auto resp = r.route({
                .url = {
                    .path = p
                }
            });

            tst::check(called, SL);
            tst::check(the_subpath == p, SL);
            tst::check(resp.status == httpmodel::status::http_200_ok, SL);
        }
    );

    suite.add<std::vector<std::string>>(
        "one_non_empty_one_level_route",
        {
            {"blah"},
            {"blah", "blah1"},
            {"blah", "blah1", "blah2"},
        },
        [](const auto& p){
            bool called = false;

            std::vector<std::string> the_subpath;

            pautina::router r(
                pautina::router::routes_type{
                    {
                        {"blah"},
                        [&](const auto& req, auto subpath){
                            called = true;
                            // std::cout << "subpath = " << subpath << std::endl;
                            the_subpath = utki::make_vector(subpath);
                            return httpmodel::response(req, httpmodel::status::http_200_ok);
                        }
                    }
                }
            );

            auto resp = r.route({
                .url = {
                    .path = p
                }
            });

            tst::check(called, SL);
            tst::check_eq(utki::make_span(const_cast<const decltype(the_subpath)&>(the_subpath)), utki::make_span(p).subspan(1), SL);
            tst::check(resp.status == httpmodel::status::http_200_ok, SL);
        }
    );
});
}
