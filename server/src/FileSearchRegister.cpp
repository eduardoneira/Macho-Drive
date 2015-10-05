#include "FileSearchRegister.h"
#include "JsonSerializer.h"
#include <algorithm>

FileSearchRegister::FileSearchRegister()
{
    //ctor
}

FileSearchRegister::~FileSearchRegister()
{
    //dtor
}

void FileSearchRegister::_setValue(){
    JsonSerializer serializer;

    std::string val_json = "";
    serializer.turnVectorToArray(this->files, "files", val_json);
    serializer.turnObjectListToObject(val_json);

    this->value = val_json;
}

void FileSearchRegister::_setValueVars(){
    Reader reader;
    Value json_value;
    reader.parse(this->value, json_value);

    for(ValueIterator it = json_value["files"].begin(); it != json_value["files"].end(); ++it){
        addFileToken((*it).asString());
    }
}

void FileSearchRegister::removeFileToken(std::string token){
    files.erase(std::remove(files.begin(), files.end(), token), files.end());
}
