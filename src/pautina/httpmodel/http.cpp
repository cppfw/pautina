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
		case status::http_100_continue:
			return "Continue"sv;
		case status::http_101_switching_protocols:
			return "Switching Protocols"sv;
		case status::http_102_processing:
			return "Processing"sv;
		case status::http_103_early_hints:
			return "Early Hints"sv;
		case status::http_200_ok:
			return "OK"sv;
		case status::http_201_created:
			return "Created"sv;
		case status::http_202_accepted:
			return "Accepted"sv;
		case status::http_203_non_authoritative_information:
			return "Non-Authoritative Information"sv;
		case status::http_204_no_content:
			return "No Content"sv;
		case status::http_205_reset_content:
			return "Reset Content"sv;
		case status::http_206_partial_content:
			return "Partial Content"sv;
		case status::http_207_multi_status:
			return "Multi-Status"sv;
		case status::http_208_already_reported:
			return "Already Reported"sv;
		case status::http_226_im_used:
			return "IM Used"sv;
		case status::http_300_multiple_choices:
			return "Multiple Choices"sv;
		case status::http_301_moved_permanently:
			return "Moved Permanently"sv;
		case status::http_302_found:
			return "Found"sv;
		case status::http_303_see_other:
			return "See Other"sv;
		case status::http_304_not_modified:
			return "Not Modified"sv;
		case status::http_305_use_proxy:
			return "Use Proxy"sv;
		case status::http_306_switch_proxy:
			return "Switch Proxy"sv;
		case status::http_307_temporary_redirect:
			return "Temporary Redirect"sv;
		case status::http_308_permanent_redirect:
			return "Permanent Redirect"sv;
		case status::http_400_bad_request:
			return "Bad Request"sv;
		case status::http_401_unauthorized:
			return "Unauthorized"sv;
		case status::http_402_payment_required:
			return "Payment Required"sv;
		case status::http_403_forbidden:
			return "Forbidden"sv;
		case status::http_404_not_found:
			return "Not Found"sv;
		case status::http_405_method_not_allowed:
			return "Method Not Allowed"sv;
		case status::http_406_not_acceptable:
			return "Not Acceptable"sv;
		case status::http_407_proxy_authentication_required:
			return "Proxy Authentication Required"sv;
		case status::http_408_request_timeout:
			return "Request Timeout"sv;
		case status::http_409_conflict:
			return "Conflict"sv;
		case status::http_410_gone:
			return "Gone"sv;
		case status::http_411_length_required:
			return "Length Required"sv;
		case status::http_412_precondition_failed:
			return "Precondition Failed"sv;
		case status::http_413_payload_too_large:
			return "Payload Too Large"sv;
		case status::http_414_uri_too_long:
			return "URI Too Long"sv;
		case status::http_415_unsupported_media_type:
			return "Unsupported Media Type"sv;
		case status::http_416_range_not_satisfiable:
			return "Range Not Satisfiable"sv;
		case status::http_417_expectation_failed:
			return "Expectation Failed"sv;
		case status::http_418_i_am_a_teapot:
			return "I'm a teapot"sv;
		case status::http_421_misdirected_request:
			return "Misdirected Request"sv;
		case status::http_422_unprocessable_entity:
			return "Unprocessable Entity"sv;
		case status::http_423_locked:
			return "Locked"sv;
		case status::http_424_failed_dependency:
			return "Failed Dependency"sv;
		case status::http_425_too_early:
			return "Too Early"sv;
		case status::http_426_upgrade_required:
			return "Upgrade Required"sv;
		case status::http_428_precondition_required:
			return "Precondition Required"sv;
		case status::http_429_too_many_requests:
			return "Too Many Requests"sv;
		case status::http_431_request_header_fields_too_large:
			return "Request Header Fields Too Large"sv;
		case status::http_451_unavailable_for_legal_reasons:
			return "Unavailable For Legal Reasons"sv;
		case status::http_500_internal_server_error:
			return "Internal Server Error"sv;
		case status::http_501_not_implemented:
			return "Not Implemented"sv;
		case status::http_502_bad_gateway:
			return "Bad Gateway"sv;
		case status::http_503_service_unavailable:
			return "Service Unavailable"sv;
		case status::http_504_gateway_timeout:
			return "Gateway Timeout"sv;
		case status::http_505_http_version_not_supported:
			return "HTTP Version Not Supported"sv;
		case status::http_506_variant_also_negotiates:
			return "Variant Also Negotiates"sv;
		case status::http_507_insufficient_storage:
			return "Insufficient Storage"sv;
		case status::http_508_loop_detected:
			return "Loop Detected"sv;
		case status::http_510_not_extended:
			return "Not Extended"sv;
		case status::http_511_network_authentication_required:
			return "Authentication Required"sv;
	}

	return {};
}
