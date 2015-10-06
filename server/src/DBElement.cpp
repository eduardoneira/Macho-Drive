#include "DBElement.h"

DBElement::DBElement(Database *datab) : key(""), value(""), db(datab)
{

}

DBElement::~DBElement()
{
    //dtor
}

void DBElement::setValue(std::string value){
    this->value = value;
    this->_setValueVars();
}

std::string DBElement::getKey(){
    this->_setKey();
    return this->key;
}

std::string DBElement::getValue(){
    this->_setValue();
    return this->value;
}
