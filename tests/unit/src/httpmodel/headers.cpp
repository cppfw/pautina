#include <tst/set.hpp>
#include <tst/check.hpp>

#include <pautina/httpmodel/headers.hpp>

namespace{
tst::set set("http__headers", [](tst::suite& suite){
    suite.add(
        "add_get",
        [](){
            httpmodel::headers h;

            h.add("Host", "blabla.com");

            tst::check(h.get("Host"), SL);
        }
    );
});
}