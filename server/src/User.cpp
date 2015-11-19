#include "User.h"
#include "UserMetadata.h"
#include "Logger.h"

#include "Database.h"

User::User(Database* db, DatabaseWriteBatch* dbbatch) : DBElement(db, dbbatch)
{
    //ctor
}

User::~User()
{
    //dtor
}

// asumo que tengo el campo username correcto
Status User::DBerase(){
    Status s;

    this->startBatch();
    // borrar metadatos
    UserMetadata user_metadata(db, this->batch);
    user_metadata.setUsername(this->getUsername());
    s = user_metadata.DBerase();
    // ver status

    s = this->erase();

    return this->endBatch();
}

Status User::DBcreate(){
    Status s;
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Corrobora en la base de datos si ya existe el usuario",INFO);
    s = this->get();
    if(!s.IsNotFound()){
        log->Log("El usuario ya existe",WARNING);
        return Status::Aborted("el usuario ya existe");
    }
    log->Log("El usuario no existe",INFO);
    this->startBatch();

    UserMetadata user_metadata(db, this->batch);
    user_metadata.setUsername(this->getUsername());
    s = user_metadata.DBcreate();
    if(!s.ok()){
        return s;
    }

    s = this->put();
    // ver status

    return this->endBatch();
}

Status User::DBget(){
    Status s;

    s = this->get();

    return s;
}
