#pragma once

#include <nitki/thread.hpp>
#include <nitki/queue.hpp>

#include <setka/tcp_socket.hpp>

#include <opros/wait_set.hpp>

#include "connection.hpp"

namespace pautina{

class connection_thread : public nitki::thread{
    setka::tcp_socket socket;

    nitki::queue queue;

    opros::wait_set wait_set;

    bool quit_flag = false;

    pautina::connection connection;

public:
    connection_thread(setka::tcp_socket&& socket);
    ~connection_thread();

    void run()override;
};

}
