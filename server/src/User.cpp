#include "User.h"
#include "UserMetadata.h"

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

    s = this->get();
    if(!s.IsNotFound()){
        return Status::Aborted("el usuario ya existe");
    }

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
