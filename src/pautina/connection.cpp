/*
MIT License

Copyright (c) 2023 Ivan Gagis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* ================ LICENSE END ================ */

#include "connection.hpp"

using namespace pautina;

connection::connection(connection_thread& owner, setka::tcp_socket&& socket) :
	owner(owner),
	socket(std::move(socket))
{}

std::vector<uint8_t> connection::send(std::vector<uint8_t>&& data)
{
	if (!this->data_to_send.empty() || data.empty()) {
		return data;
	}

	this->data_to_send = std::move(data);
	this->num_bytes_sent = 0;

	this->status.set(opros::ready::write);

	return {};
}

void connection::set_can_receive_data(bool can_receive)
{
	this->status.set(opros::ready::read, can_receive);
}
