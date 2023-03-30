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

#include <string_view>

namespace pautina::http {

enum class protocol {
	http_1_0,
	http_1_1,
	http_2_0
};

/**
 * @brief Parse request protocol from string.
 * @param str - string to parse request protocol from.
 * @return Parsed request protocol.
 * @throw std::invalid_argument in case of unknown protocol is given in the input string.
 */
protocol protocol_from_string(std::string_view str);

/**
 * @brief Get string name for request protocol.
 * @param p - request protocol to get the string name for.
 * @return String name of the request protocol.
 */
std::string_view protocol_to_string(protocol p) noexcept;

/**
 * @brief Request method.
 */
enum class method {
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

/**
 * @brief Parse request method from string.
 * @param str - string to parse request method from.
 * @return Parsed request method.
 * @throw std::invalid_argument in case unknown request method is given in the input string.
 */
method method_from_string(std::string_view str);

/**
 * @brief Get string name for request method.
 * @param m - request method to get string name for.
 * @return String name of the request method.
 */
std::string_view method_to_string(method m) noexcept;

} // namespace pautina::http
