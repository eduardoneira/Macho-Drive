#include "Database.h"
#include "rocksdb/options.h"

using namespace rocksdb;

Database::Database() : db(NULL), db_path("")
{

}

Database::~Database()
{
    this->close();
}

Status Database::config(const std::string& db_path){
    if(this->db != NULL){
        return Status::Busy(); // agregar msg de error?
    }
    this->db_path = db_path; // chequear si es valido?
    return Status::OK();
}

Status Database::open(){
    if(db_path.compare("") == 0){
        return Status::NotFound();
    }
    if(db != NULL){
        return Status::Busy();
    }

    Options options;
    options.create_if_missing = true;

    return DB::Open(options, db_path, &db);
}

void Database::close(){
    if(db != NULL){
        delete db;
        db = NULL;
    }
}

Status Database::put(DBElement &elem){
    if(db == NULL)
        return Status::NotFound();
    return db->Put(WriteOptions(), elem.getKey(), elem.getValue());
}

Status Database::get(DBElement &elem){
    if(db == NULL)
        return Status::NotFound();
    std::string get_result;
    //std::cout << "elem key antes: " << elem.getKeyToString() << std::endl;
    //std::cout << "elem val antes: " << elem.getValueToString() << std::endl;
    Status s = db->Get(ReadOptions(), elem.getKey(), &get_result);
    //std::cout << "get_res: " << get_result << std::endl;
    elem.setValue(get_result);
    //std::cout << "elem key despues: " << elem.getKeyToString() << std::endl;
    //std::cout << "elem val despues: " << elem.getValueToString() << std::endl;

    return s;
}

Status Database::erase(DBElement &elem){
    if(db == NULL)
        return Status::NotFound();
    return db->Delete(WriteOptions(), elem.getKey());
}

Status Database::clear_all(){
    if(db == NULL)
        return Status::NotFound();
    Status s;
    rocksdb::Iterator* it = db->NewIterator(ReadOptions());
    for(it->SeekToFirst(); it->Valid(); it->Next()){
        s = db->Delete(WriteOptions(), it->key());
        if(!s.ok()){
            return s;
        }
    }
    if(!it->status().ok()){
        return it->status();
    }
    delete it;
    return s;
}
