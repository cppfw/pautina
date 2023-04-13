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
enum class status {
	http_100_continue = 100,
	http_101_switching_protocols = 101,
	http_102_processing = 102,
	http_103_early_hints = 103,
	http_200_ok = 200,
	http_201_created = 201,
	http_202_accepted = 202,
	http_203_non_authoritative_information = 203,
	http_204_no_content = 204,
	http_205_reset_content = 205,
	http_206_partial_content = 206,
	http_207_multi_status = 207,
	http_208_already_reported = 208,
	http_226_im_used = 226,
	http_300_multiple_choices = 300,
	http_301_moved_permanently = 301,
	http_302_found = 302,
	http_303_see_other = 303,
	http_304_not_modified = 304,
	http_305_use_proxy = 305,
	http_306_switch_proxy = 306,
	http_307_temporary_redirect = 307,
	http_308_permanent_redirect = 308,
	http_400_bad_request = 400,
	http_401_unauthorized = 401,
	http_402_payment_required = 402,
	http_403_forbidden = 403,
	http_404_not_found = 404,
	http_405_method_not_allowed = 405,
	http_406_not_acceptable = 406,
	http_407_proxy_authentication_required = 407,
	http_408_request_timeout = 408,
	http_409_conflict = 409,
	http_410_gone = 410,
	http_411_length_required = 411,
	http_412_precondition_failed = 412,
	http_413_payload_too_large = 413,
	http_414_uri_too_long = 414,
	http_415_unsupported_media_type = 415,
	http_416_range_not_satisfiable = 416,
	http_417_expectation_failed = 417,
	http_418_i_am_a_teapot = 418,
	http_421_misdirected_request = 421,
	http_422_unprocessable_entity = 422,
	http_423_locked = 423,
	http_424_failed_dependency = 424,
	http_425_too_early = 425,
	http_426_upgrade_required = 426,
	http_428_precondition_required = 428,
	http_429_too_many_requests = 429,
	http_431_request_header_fields_too_large = 431,
	http_451_unavailable_for_legal_reasons = 451,
	http_500_internal_server_error = 500,
	http_501_not_implemented = 501,
	http_502_bad_gateway = 502,
	http_503_service_unavailable = 503,
	http_504_gateway_timeout = 504,
	http_505_http_version_not_supported = 505,
	http_506_variant_also_negotiates = 506,
	http_507_insufficient_storage = 507,
	http_508_loop_detected = 508,
	http_510_not_extended = 510,
	http_511_network_authentication_required = 511,
};

std::string to_string(status s);

std::string_view get_status_text(status s);

} // namespace httpmodel
