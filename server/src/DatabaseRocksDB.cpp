#include "DatabaseRocksDB.h"
#include "rocksdb/options.h"

using namespace rocksdb;

DatabaseRocksDB::DatabaseRocksDB() : db(NULL), db_path("")
{

}

DatabaseRocksDB::~DatabaseRocksDB()
{
    this->close();
}

Status DatabaseRocksDB::config(const std::string& db_path){
    if(this->db != NULL){
        return Status::Busy(); // agregar msg de error?
    }
    this->db_path = db_path; // chequear si es valido?
    return Status::OK();
}

Status DatabaseRocksDB::open(){
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

void DatabaseRocksDB::close(){
    if(db != NULL){
        delete db;
        db = NULL;
    }
}

Status DatabaseRocksDB::put(DBElement &elem){
    if(db == NULL)
        return Status::NotFound("No se inicializo la base de datos");
    Status s = db->Put(WriteOptions(), elem.getKey(), elem.getValue());
    if(!s.ok())
        return Status::Aborted("Error interno en la base de datos al guardar el registro");
    return s;
}

Status DatabaseRocksDB::get(DBElement &elem){
    if(db == NULL)
        return Status::NotFound("la base de datos no fue creada");
    std::string get_result;
    //std::cout << "elem key antes: " << elem.getKeyToString() << std::endl;
    //std::cout << "elem val antes: " << elem.getValueToString() << std::endl;
    Status s = db->Get(ReadOptions(), elem.getKey(), &get_result);
    if(!s.ok()){
        return Status::NotFound("Error interno en la base de datos al buscar el registro");
    }
    //std::cout << "get_res: " << get_result << std::endl;
    elem.setValue(get_result);
    //std::cout << "elem key despues: " << elem.getKeyToString() << std::endl;
    //std::cout << "elem val despues: " << elem.getValueToString() << std::endl;

    return s;
}

Status DatabaseRocksDB::erase(DBElement &elem){
    if(db == NULL)
        return Status::NotFound();
    return db->Delete(WriteOptions(), elem.getKey());
}

Status DatabaseRocksDB::clear_all(){
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
