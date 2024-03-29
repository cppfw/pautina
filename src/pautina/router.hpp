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

#include <functional>
#include <map>

#include <httpmodel/request.hpp>
#include <httpmodel/response.hpp>
#include <utki/span.hpp>

namespace pautina {

using route_handler_type = std::function< //
	httpmodel::response( //
		const httpmodel::request& request, //
		utki::span<const std::string> subpath //
	) //
	>;

class router
{
public:
	using routes_type = std::map<std::vector<std::string>, route_handler_type, urlmodel::path_less>;

private:
	const routes_type routes;

public:
	router(routes_type routes);

	// thread-safe:
	// it is ok to call this method from concurrent threads, because it does not modify
	// the routes map
	httpmodel::response route(const httpmodel::request& req) const;
};

} // namespace pautina
