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

#pragma once

#include <setka/tcp_socket.hpp>
#include <utki/flags.hpp>
#include <utki/span.hpp>

#include "httpmodel/request_parser.hpp"

namespace pautina {

class connection
{
	friend class connection_thread;

	setka::tcp_socket socket;

public:
	connection(setka::tcp_socket&& socket);

	virtual ~connection() = default;

private:
	// initially connection is ready to receive data
	utki::flags<opros::ready> status{opros::ready::read};

	// stuff used in 'sending' state
	std::vector<uint8_t> data_to_send;
	size_t num_bytes_sent;

protected:
	/**
	 * @brief Turn on/off handle_received_data() notifications.
	 * Not thread-safe.
	 * @param can_receive - whether to turn on (true) or off (false) the handle_received_data() notifications.
	 */
	void set_can_receive_data(bool can_receive);

public:
	/**
	 * @return true if ready to handle more data.
	 * @return true if not ready to handle more data.
	 */
	virtual void handle_received_data(utki::span<const uint8_t> data) = 0;

	/**
	 * @return true if ready to receive more data.
	 * @return true if not ready to receive data.
	 */
	virtual void handle_data_sent() = 0;

	/**
	 * @brief Check if send buffer is empty.
	 * Not thread-safe.
	 * @return true if send buffer is free and data can be sent over the connection.
	 * @return false otherwise.
	 */
	bool can_send() const noexcept
	{
		return !this->status.get(opros::ready::write);
	}

	/**
	 * @brief Send data over the connection.
	 * @param data - data to send.
	 * @return Empty vector in case the data has been accepted for sending.
	 * @return Passed in vector in case send buffer is full.
	 */
	// TODO: should be thread-safe?
	std::vector<uint8_t> send(std::vector<uint8_t>&& data);
};

} // namespace pautina
