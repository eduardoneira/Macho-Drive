#include "JsonSerializer.h"

using namespace Json;

JsonSerializer::JsonSerializer()
{
    //ctor
}

JsonSerializer::~JsonSerializer()
{
    //dtor
}

std::string JsonSerializer::get(Value value, std::string key, std::string default_val, Value &val, std::string &str_val){
    val = value.get(key, default_val);
    std::string temp_str = val.toStyledString();
    if(temp_str[0] == '"'){
        str_val = temp_str.substr(1, temp_str.size()-3);
    } else {
        str_val = temp_str;
    }
    return str_val;
}

std::string JsonSerializer::get(Value value, int key, std::string default_val, Value &val, std::string &str_val){
    val = value.get(key, default_val);
    std::string temp_str = val.toStyledString();
    if(temp_str[0] == '"'){
        str_val = temp_str.substr(1, temp_str.size()-3);
    } else {
        str_val = temp_str;
    }
    return str_val;
}

void JsonSerializer::turnObjectListToObject(std::string &lista){
    turnObjectListToObject(lista, "");
}

void JsonSerializer::turnObjectListToObject(std::string &lista, std::string name){
    std::string str_to_add = "";
    if(name != ""){
        str_to_add.append("\"");
        str_to_add.append(name);
        str_to_add.append("\"");

        str_to_add.append(" : ");
    }

    str_to_add.append("{ ");

    //std::string str_to_add_at_end = "";
    //str_to_add_at_end.append(" ]");
    str_to_add.append(lista);
    str_to_add.append(" }");
    lista = str_to_add;
}

void JsonSerializer::turnArrayListToArray(std::string &lista, std::string name){
    std::string str_to_add = "";
    if(name != ""){
        str_to_add.append("\"");
        str_to_add.append(name);
        str_to_add.append("\"");

        str_to_add.append(" : ");
    }

    str_to_add.append("[ ");

    //std::string str_to_add_at_end = "";
    //str_to_add_at_end.append(" ]");
    str_to_add.append(lista);
    str_to_add.append(" ]");
    lista = str_to_add;
}

void JsonSerializer::turnArrayListToArray(std::string &lista){
    turnArrayListToArray(lista, "");
}

void JsonSerializer::addValueToObjectList(std::string &lista, std::string name, std::string value){
    std::string str_to_add = "";

    if(lista != ""){
        str_to_add.append(", ");
    }

    str_to_add.append("\"");
    str_to_add.append(name);
    str_to_add.append("\"");

    str_to_add.append(" : ");
    str_to_add.append("\"");
    str_to_add.append(value);
    str_to_add.append("\"");

    lista.append(str_to_add);
}

void JsonSerializer::addValueToArrayList(std::string &lista, std::string value){
    std::string str_to_add = "";

    if(lista != ""){
        str_to_add.append(", ");
    }

    str_to_add.append("\"");
    str_to_add.append(value);
    str_to_add.append("\"");

    lista.append(str_to_add);
}

void JsonSerializer::addArrayOrObjectToList(std::string &lista, std::string name, std::string value){
    std::string str_to_add = "";

    if(lista != ""){
        str_to_add.append(", ");
    }


    str_to_add.append("\"");
    str_to_add.append(name);
    str_to_add.append("\"");
    str_to_add.append(" : ");
    str_to_add.append(value);

    lista.append(str_to_add);
}

void JsonSerializer::joinValueIntoList(std::string &lista, std::string value){
    std::string str_to_add = "";
    if(lista != ""){
        str_to_add.append(", ");
    }

    str_to_add.append(value);
    lista.append(str_to_add);
}

void JsonSerializer::turnVectorToArray(std::vector<std::string>& vec, std::string name, std::string &json){
    std::string array_list = "";
    for(std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it){
        this->addValueToArrayList(array_list, *it);
    }
    this->turnArrayListToArray(array_list, name);
    json = array_list;
}

void JsonSerializer::addNumberToList(std::string &lista, std::string name, int number){
    std::string str_to_add = "";

    if(lista != ""){
        str_to_add.append(", ");
    }

    str_to_add.append("\"");
    str_to_add.append(name);
    str_to_add.append("\"");

    str_to_add.append(" : ");
    str_to_add.append(std::to_string(number));

    lista.append(str_to_add);
}

void JsonSerializer::addBoolToList(std::string &lista, std::string name, bool boolean){
    std::string str_to_add = "";

    if(lista != ""){
        str_to_add.append(", ");
    }

    str_to_add.append("\"");
    str_to_add.append(name);
    str_to_add.append("\"");

    str_to_add.append(" : ");
    if(boolean == true){
        str_to_add.append("true");
    } else {
        str_to_add.append("false");
    }

    lista.append(str_to_add);
}
