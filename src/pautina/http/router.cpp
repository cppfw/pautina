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

#include "router.hpp"

using namespace pautina::http;

router::router(decltype(routes)&& routes) :
	routes(std::move(routes))
{}

httpmodel::response router::route(const httpmodel::request& req) const
{
	auto path = utki::make_span(req.url.path);

	while (true) {
		auto i = this->routes.find(path);
		if (i != this->routes.end()) {
			return i->second(req, utki::make_span(req.url.path).subspan(path.size()));
		}

		if (path.empty()) {
			break;
		}

		ASSERT(path.size() != 0)

		path = path.subspan(0, path.size() - 1);
	}

	return {.status = httpmodel::status_code::http_404};
}
