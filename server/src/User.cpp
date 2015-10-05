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
