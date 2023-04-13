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

#include "http.hpp"

#include <map>
#include <sstream>

#include <utki/debug.hpp>
#include <utki/util.hpp>

using namespace std::string_view_literals;

using namespace httpmodel;

namespace {
const std::map<std::string_view, protocol> protocol_name_to_enum_mapping = {
	{"HTTP/1.0", protocol::http_1_0},
	{"HTTP/1.1", protocol::http_1_1},
	{"HTTP/2.0", protocol::http_2_0},
};

const auto protocol_enum_to_name_mapping = utki::flip_map(protocol_name_to_enum_mapping);
} // namespace

protocol httpmodel::protocol_from_string(std::string_view str)
{
	auto i = protocol_name_to_enum_mapping.find(str);
	if (i == protocol_name_to_enum_mapping.end()) {
		std::stringstream ss;
		ss << "unknown request protocol: '" << str << '\'';
		throw std::invalid_argument(ss.str());
	}

	return i->second;
}

std::string_view httpmodel::to_string(protocol p) noexcept
{
	auto i = protocol_enum_to_name_mapping.find(p);
	ASSERT(i != protocol_enum_to_name_mapping.end())
	return i->second;
}

namespace {
const std::map<std::string_view, method> method_name_to_enum_mapping = {
	{    "GET",       method::get},
	{   "HEAD",      method::head},
	{   "POST",      method::post},
	{    "PUT",       method::put},
	{ "DELETE", method::delete_nk},
	{"CONNECT",   method::connect},
	{"OPTIONS",   method::options},
	{  "TRACE",     method::trace},
	{  "PATCH",     method::patch}
};

const auto method_enum_to_name_mapping = utki::flip_map(method_name_to_enum_mapping);
} // namespace

method httpmodel::method_from_string(std::string_view str)
{
	auto i = method_name_to_enum_mapping.find(str);
	if (i == method_name_to_enum_mapping.end()) {
		std::stringstream ss;
		ss << "unknown request method: " << str;
		throw std::invalid_argument(ss.str());
	}
	return i->second;
}

std::string_view httpmodel::to_string(method m) noexcept
{
	auto i = method_enum_to_name_mapping.find(m);
	ASSERT(i != method_enum_to_name_mapping.end())
	return i->second;
}

std::string httpmodel::to_string(status sc)
{
	return std::to_string(unsigned(sc));
}

std::string_view httpmodel::get_status_text(status sc)
{
	switch (sc) {
		case status::http_100:
			return "Continue"sv;
		case status::http_101:
			return "Switching Protocols"sv;
		case status::http_102:
			return "Processing"sv;
		case status::http_103:
			return "Early Hints"sv;
		case status::http_200:
			return "OK"sv;
		case status::http_201:
			return "Created"sv;
		case status::http_202:
			return "Accepted"sv;
		case status::http_203:
			return "Non-Authoritative Information"sv;
		case status::http_204:
			return "No Content"sv;
		case status::http_205:
			return "Reset Content"sv;
		case status::http_206:
			return "Partial Content"sv;
		case status::http_207:
			return "Multi-Status"sv;
		case status::http_208:
			return "Already Reported"sv;
		case status::http_226:
			return "IM Used"sv;
		case status::http_300:
			return "Multiple Choices"sv;
		case status::http_301:
			return "Moved Permanently"sv;
		case status::http_302:
			return "Found"sv;
		case status::http_303:
			return "See Other"sv;
		case status::http_304:
			return "Not Modified"sv;
		case status::http_305:
			return "Use Proxy"sv;
		case status::http_306:
			return "Switch Proxy"sv;
		case status::http_307:
			return "Temporary Redirect"sv;
		case status::http_308:
			return "Permanent Redirect"sv;
		case status::http_400:
			return "Bad Request"sv;
		case status::http_401:
			return "Unauthorized"sv;
		case status::http_402:
			return "Payment Required"sv;
		case status::http_403:
			return "Forbidden"sv;
		case status::http_404:
			return "Not Found"sv;
		case status::http_405:
			return "Method Not Allowed"sv;
		case status::http_406:
			return "Not Acceptable"sv;
		case status::http_407:
			return "Proxy Authentication Required"sv;
		case status::http_408:
			return "Request Timeout"sv;
		case status::http_409:
			return "Conflict"sv;
		case status::http_410:
			return "Gone"sv;
		case status::http_411:
			return "Length Required"sv;
		case status::http_412:
			return "Precondition Failed"sv;
		case status::http_413:
			return "Payload Too Large"sv;
		case status::http_414:
			return "URI Too Long"sv;
		case status::http_415:
			return "Unsupported Media Type"sv;
		case status::http_416:
			return "Range Not Satisfiable"sv;
		case status::http_417:
			return "Expectation Failed"sv;
		case status::http_418:
			return "I'm a teapot"sv;
		case status::http_421:
			return "Misdirected Request"sv;
		case status::http_422:
			return "Unprocessable Entity"sv;
		case status::http_423:
			return "Locked"sv;
		case status::http_424:
			return "Failed Dependency"sv;
		case status::http_425:
			return "Too Early"sv;
		case status::http_426:
			return "Upgrade Required"sv;
		case status::http_428:
			return "Precondition Required"sv;
		case status::http_429:
			return "Too Many Requests"sv;
		case status::http_431:
			return "Request Header Fields Too Large"sv;
		case status::http_451:
			return "Unavailable For Legal Reasons"sv;
		case status::http_500:
			return "Internal Server Error"sv;
		case status::http_501:
			return "Not Implemented"sv;
		case status::http_502:
			return "Bad Gateway"sv;
		case status::http_503:
			return "Service Unavailable"sv;
		case status::http_504:
			return "Gateway Timeout"sv;
		case status::http_505:
			return "HTTP Version Not Supported"sv;
		case status::http_506:
			return "Variant Also Negotiates"sv;
		case status::http_507:
			return "Insufficient Storage"sv;
		case status::http_508:
			return "Loop Detected"sv;
		case status::http_510:
			return "Not Extended"sv;
		case status::http_511:
			return "Authentication Required"sv;
	}

	return {};
}
