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

using namespace pautina::http;

namespace {
const std::map<std::string_view, protocol> protocol_name_to_enum_mapping = {
	{"HTTP/1.0", protocol::http_1_0},
	{"HTTP/1.1", protocol::http_1_1},
	{"HTTP/2.0", protocol::http_2_0},
};

const auto protocol_enum_to_name_mapping = utki::flip_map(protocol_name_to_enum_mapping);
} // namespace

protocol pautina::http::protocol_from_string(std::string_view str)
{
	auto i = protocol_name_to_enum_mapping.find(str);
	if (i == protocol_name_to_enum_mapping.end()) {
		std::stringstream ss;
		ss << "unknown request protocol: " << str;
		throw std::invalid_argument(ss.str());
	}

	return i->second;
}

std::string_view pautina::http::protocol_to_string(protocol p) noexcept
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

method pautina::http::method_from_string(std::string_view str)
{
	auto i = method_name_to_enum_mapping.find(str);
	if (i == method_name_to_enum_mapping.end()) {
		std::stringstream ss;
		ss << "unknown request method: " << str;
		throw std::invalid_argument(ss.str());
	}
	return i->second;
}

std::string_view pautina::http::method_to_string(method m) noexcept
{
	auto i = method_enum_to_name_mapping.find(m);
	ASSERT(i != method_enum_to_name_mapping.end())
	return i->second;
}
