#pragma once

#include <utki/span.hpp>

namespace pautina{

class connection{
public:
    // return true if expect more data
    // return false otherwise
    bool on_data_received(utki::span<const uint8_t> data);

    std::vector<uint8_t> get_data_to_send();
};

}
