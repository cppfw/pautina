#include "connection_thread.hpp"

using namespace pautina;

connection_thread::connection_thread(setka::tcp_socket&& socket) :
    socket(std::move(socket)),
    wait_set(2)
{
    this->wait_set.add(this->socket, opros::ready::read | opros::ready::write);
    this->wait_set.add(this->queue, opros::ready::read);
}

connection_thread::~connection_thread(){
    this->wait_set.remove(this->queue);
    this->wait_set.remove(this->socket);
}

void connection_thread::run(){
    while(!this->quit_flag){
        this->wait_set.wait();

        if(this->queue.flags().get(opros::ready::read)){
            while(auto proc = this->queue.pop_front()){
                proc.operator()();
            }
        }

        if(this->socket.flags().get(opros::ready::read)){
            // TODO: read
        }

        if(this->socket.flags().get(opros::ready::write)){
            // TODO: write
        }
    }
}
