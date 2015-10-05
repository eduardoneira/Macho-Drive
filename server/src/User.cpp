#include "User.h"
#include "UserMetadata.h"

#include "Database.h"

User::User(Database* db) : DBElement(db)
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

    // borrar metadatos
    UserMetadata user_metadata(db);
    user_metadata.setUsername(this->getUsername());
    s = user_metadata.DBerase();
    // ver status

    s = this->db->erase(*this);
    return s;
}

Status User::DBcreate(){
    Status s;

    s = this->db->get(*this);
    // ver status, si ya existe devolver error
    UserMetadata user_metadata(db);
    user_metadata.setUsername(this->getUsername());
    s = user_metadata.DBcreate();
    // ver status, si ya existe devolver error
    s = this->db->put(*this);
    // ver status
    return s;
}

Status User::DBget(){
    Status s;

    this->db->get(*this);

    return s;
}
