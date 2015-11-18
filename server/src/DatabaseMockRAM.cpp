#include "DatabaseMockRAM.h"
#include <iostream>
#include "DatabaseWriteBatch.h"

using namespace rocksdb;

DatabaseMockRAM::DatabaseMockRAM(){
    //ctor
}

DatabaseMockRAM::~DatabaseMockRAM(){
    //dtor
}

Status DatabaseMockRAM::config(const std::string& db_path, bool create_if_missing){
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
    return this->erase(elem.getKey());
}

Status DatabaseMockRAM::erase(std::string key){
    if(db.erase(key) == 0){
        return Status::NotFound();
    }
    return Status::OK();
}

Status DatabaseMockRAM::put(DBElement& elem){
    return this->put(elem.getKey(), elem.getValue());
}

Status DatabaseMockRAM::put(std::string key, std::string value){
    db[key] = value;
    return Status::OK();
}

Status DatabaseMockRAM::get(DBElement& elem){
    std::map<std::string, std::string>::iterator it = db.find(elem.getKey());
    if(it != db.end()){
        elem.setValue(it->second);
        return Status::OK();
    } else {
        //elem.setValue("");
        return Status::NotFound();
    }
}

Status DatabaseMockRAM::writeBatch(DatabaseWriteBatch *batch){

    std::vector<std::string> *keys = batch->getKeys();
    std::vector<std::string> *values = batch->getValues();
    std::vector<std::string> *operations = batch->getOperations();
    Status s;
    for(int i = 0, size = operations->size(); i < size; ++i){
        if(operations->at(i) == "erase"){
            s = this->erase(keys->at(i));
        } else if(operations->at(i) == "put"){
            s = this->put(keys->at(i), values->at(i));
        } else {
            //error
        }
    }

    return s;
}
