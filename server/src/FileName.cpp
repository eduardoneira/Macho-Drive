#include "FileName.h"

FileName::FileName(Database* db) : FileSearchRegister(db)
{
    //ctor
}

FileName::~FileName()
{
    //dtor
}

void FileName::_setKey(){
    this->key = this->username;
    this->key.append(".");
    this->key.append(this->name);
}
