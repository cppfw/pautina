#pragma once

namespace pautina::http{

enum class protocol{
    http_1_0,
    http_1_1,
    http_2_0
};

/**
 * @brief Request method.
 */
enum class method{
    get,
    head,
    post,
    put,
    delete_resource,
    connect,
    options,
    trace,
    patch
};

}
