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

namespace {
bool is_less(const std::vector<std::string>& a, const std::vector<std::string>& b)
{
	auto i = a.begin();
	auto j = b.begin();

	for (; i != a.end() && j != b.end(); ++i, ++j) {
		if (a < b) {
			continue;
		}
		break;
	}

	return a.size() < b.size();
}
} // namespace

router::router(decltype(sorted_routes)&& routes) :
	sorted_routes(std::move(routes))

{
	std::sort( //
		this->sorted_routes.begin(),
		this->sorted_routes.end(),
		[](const auto& a, const auto& b) {
			return is_less(a.path, b.path);
		}
	);
}
