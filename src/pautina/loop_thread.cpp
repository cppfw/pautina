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

#include "loop_thread.hpp"

#include <sstream>

using namespace nitki;

loop_thread::loop_thread(unsigned wait_set_capacity) :
	wait_set([&]() {
		auto max = std::numeric_limits<decltype(wait_set_capacity)>::max();
		if (wait_set_capacity == std::numeric_limits<decltype(wait_set_capacity)>::max()) {
			std::stringstream ss;
			ss << "loop_thread::loop_thread(): wait_set_capacity must be less "
				  "than "
			   << max;
			throw std::invalid_argument(ss.str());
		}
		return wait_set_capacity + 1;
	}())
{}

void loop_thread::quit() noexcept
{
	this->quit_flag.store(true);
	this->queue.poke();
}

void loop_thread::run()
{
	std::vector<opros::event_info> triggered(this->wait_set.capacity());

	std::optional<uint32_t> timeout = this->on_loop(nullptr);

	while (!this->quit_flag.load()) {
		auto num_triggered =
			timeout.has_value() ? this->wait_set.wait(timeout.value(), triggered) : this->wait_set.wait(triggered);

		while (auto proc = this->queue.pop_front()) {
			proc.operator()();
		}

		timeout = this->on_loop(utki::make_span(triggered.data(), num_triggered));
	}
}
