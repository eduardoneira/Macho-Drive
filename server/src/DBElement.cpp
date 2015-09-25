#include "DBElement.h"

DBElement::DBElement() : key(""), value("")
{

}

DBElement::~DBElement()
{
    //dtor
}

std::string DBElement::getKey(){
    this->_setKey();
    return this->key;
}

std::string DBElement::getValue(){
    this->_setValue();
    return this->value;
}
