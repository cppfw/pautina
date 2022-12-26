#pragma once

#include <string>
#include <functional>

namespace pautina{

class http_server{
public:

    void add(
        std::vector<std::string> path,
        std::function<void()> handler
    );
};

}
