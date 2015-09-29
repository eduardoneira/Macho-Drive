#include "DatabaseMockRAM.h"
#include <iostream>

using namespace rocksdb;

DatabaseMockRAM::DatabaseMockRAM(){
    //ctor
}

DatabaseMockRAM::~DatabaseMockRAM(){
    //dtor
}

Status DatabaseMockRAM::config(const std::string& db_path){
    return Status::OK();
}

Status DatabaseMockRAM::open(){
    return Status::OK();
}

void DatabaseMockRAM::close(){

}

Status DatabaseMockRAM::clear_all(){
    db.clear();
    return Status::OK();
}

Status DatabaseMockRAM::erase(DBElement& elem){
    db.erase(elem.getKey());
    return Status::OK();
}

Status DatabaseMockRAM::put(DBElement& elem){
    db.insert(std::pair<std::string, std::string>(elem.getKey(), elem.getValue()));
    return Status::OK();
}

Status DatabaseMockRAM::get(DBElement& elem){
    std::map<std::string, std::string>::iterator it = db.find(elem.getKey());
    if(it != db.end())
        elem.setValue(it->second);
    else
        elem.setValue("not found");
    return Status::OK();
}
