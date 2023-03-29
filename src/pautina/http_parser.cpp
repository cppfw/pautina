#include "http_parser.hpp"

#include <stdexcept>
#include <locale>
#include <sstream>

#include <utki/string.hpp>

using namespace pautina;

std::string_view http_parser::parse_idle(std::string_view str){
    if(str.empty()){
        return str;
    }

    if(std::isspace(str.front(), std::locale::classic())){
        throw std::invalid_argument("http_parser: unexpected whitespace while parsing request method");
    }
    
    this->cur_state = state::method;
    return str;
}

std::string_view http_parser::parse_skip_spaces(std::string_view str){
    for(auto i = str.begin(); i != str.end(); ++i){
        auto c = *i;
        if(c != ' '){
            str = str.substr(std::distance(str.begin(), i) + 1);
            this->cur_state = this->state_after_skiping_spaces;
            break;
        }
    }
    return str;
}

namespace{
const std::map<std::string_view, http::method, std::less<>> method_name_to_enum_mapping = {
    {"GET", http::method::get},
    {"HEAD", http::method::head},
    {"POST", http::method::post},
    {"PUT", http::method::put},
    {"DELETE", http::method::delete_resource},
    {"CONNECT", http::method::connect},
    {"OPTIONS", http::method::options},
    {"TRACE", http::method::trace},
    {"PATCH", http::method::patch}
};
}

std::string_view http_parser::parse_method(std::string_view str){
    for(auto i = str.begin(); i != str.end(); ++i){
        auto c = *i;

        if(c == ' '){
            // method name read

            {
                auto method_name = utki::make_string_view(this->buf);
                auto i = method_name_to_enum_mapping.find(method_name);
                if(i == method_name_to_enum_mapping.end()){
                    std::stringstream ss;
                    ss << "http_parser: unknown request method: " << method_name;
                    throw std::invalid_argument(ss.str());
                }
                this->method = i->second;
            }

            str = str.substr(std::distance(str.begin(), i) + 1);
            this->cur_state = state::skip_spaces;
            this->state_after_skiping_spaces = state::path;
            break;
        }else{
            this->buf.push_back(std::toupper(c, std::locale::classic()));
        }
    }
    return str;
}

std::string_view http_parser::feed(std::string_view str){
    while(!str.empty()){
        switch(this->cur_state){
            case state::idle:
                str = this->parse_idle(str);
                break;
            case state::skip_spaces:
                str = this->parse_skip_spaces(str);
                break;
            case state::method:
                str = this->parse_method(str);
                break;
            case state::path:
                // TODO:
                break;
            case state::end:
                return str;
        }
    }
    return str;
}
