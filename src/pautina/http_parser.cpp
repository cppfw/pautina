#include "http_parser.hpp"

#include <stdexcept>
#include <locale>

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

std::string_view http_parser::feed(std::string_view str){
    while(!str.empty()){
        switch(this->cur_state){
            case state::idle:
                str = this->parse_idle(str);
                break;
            case state::method:
                // TODO:
                break;
            case state::end:
                return str;
        }
    }
    return str;
}
