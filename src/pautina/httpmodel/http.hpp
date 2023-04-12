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

namespace httpmodel {

enum class protocol {
	// order is ascending by protocol version
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
std::string_view to_string(protocol p) noexcept;

/**
 * @brief Request method.
 */
enum class method {
	get,
	head,
	post,
	put,
	delete_nk,
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
std::string_view to_string(method m) noexcept;

/**
 * @brief HTTP response status code.
 */
enum class status_code {
	http_100 = 100, // Continue
	http_101 = 101, // Switching Protocols
	http_102 = 102, // Processing
	http_103 = 103, // Early Hints
	http_200 = 200, // OK
	http_201 = 201, // Created
	http_202 = 202, // Accepted
	http_203 = 203, // Non-Authoritative Information
	http_204 = 204, // No Content
	http_205 = 205, // Reset Content
	http_206 = 206, // Partial Content
	http_207 = 207, // Multi-Status
	http_208 = 208, // Already Reported
	http_226 = 226, // IM Used
	http_300 = 300, // Multiple Choices
	http_301 = 301, // Moved Permanently
	http_302 = 302, // Found
	http_303 = 303, // See Other
	http_304 = 304, // Not Modified
	http_305 = 305, // Use Proxy
	http_306 = 306, // Switch Proxy
	http_307 = 307, // Temporary Redirect
	http_308 = 308, // Permanent Redirect
	http_400 = 400, // Bad Request
	http_401 = 401, // Unauthorized
	http_402 = 402, // Payment Required
	http_403 = 403, // Forbidden
	http_404 = 404, // Not Found
	http_405 = 405, // Method Not Allowed
	http_406 = 406, // Not Acceptable
	http_407 = 407, // Proxy Authentication Required
	http_408 = 408, // Request Timeout
	http_409 = 409, // Conflict
	http_410 = 410, // Gone
	http_411 = 411, // Length Required
	http_412 = 412, // Precondition Failed
	http_413 = 413, // Payload Too Large
	http_414 = 414, // URI Too Long
	http_415 = 415, // Unsupported Media Type
	http_416 = 416, // Range Not Satisfiable
	http_417 = 417, // Expectation Failed
	http_418 = 418, // I'm a teapot
	http_421 = 421, // Misdirected Request
	http_422 = 422, // Unprocessable Entity
	http_423 = 423, // Locked
	http_424 = 424, // Failed Dependency
	http_425 = 425, // Too Early
	http_426 = 426, // Upgrade Required
	http_428 = 428, // Precondition Required
	http_429 = 429, // Too Many Requests
	http_431 = 431, // Request Header Fields Too Large
	http_451 = 451, // Unavailable For Legal Reasons
	http_500 = 500, // Internal Server Error
	http_501 = 501, // Not Implemented
	http_502 = 502, // Bad Gateway
	http_503 = 503, // Service Unavailable
	http_504 = 504, // Gateway Timeout
	http_505 = 505, // HTTP Version Not Supported
	http_506 = 506, // Variant Also Negotiates
	http_507 = 507, // Insufficient Storage
	http_508 = 508, // Loop Detected
	http_510 = 510, // Not Extended
	http_511 = 511 // Network Authentication Required
};

std::string to_string(status_code sc);

std::string_view get_status_text(status_code sc);

} // namespace httpmodel
