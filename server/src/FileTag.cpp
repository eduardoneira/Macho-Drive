#include "FileTag.h"

FileTag::FileTag(Database* db) : FileSearchRegister(db)
{
    //ctor
}

FileTag::~FileTag()
{
    //dtor
}

void FileTag::_setKey(){
    this->key = this->username;
    this->key.append(".");
    this->key.append(this->tag);
}
