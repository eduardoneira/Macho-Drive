#include "Database.h"
#include "rocksdb/options.h"

using namespace rocksdb;

Database::Database() : db(NULL), db_path("")
{

}

Database::~Database()
{
    if(db != NULL){
        delete this->db;
    }
}

Status Database::config(const std::string& db_path){
    if(this->db != NULL){
        return Status::Busy(); // agregar msg de error?
    }
    this->db_path = db_path; // chequear si es valido?
    return Status::OK();
}

Status Database::create(){
    if(db_path.compare("") == 0){
        return Status::NotFound();
    }

    Options options;
    options.create_if_missing = true;

    return DB::Open(options, db_path, &db);
}
