#include "FileExtension.h"

FileExtension::FileExtension(Database* db) : FileSearchRegister(db)
{
    //ctor
}

FileExtension::~FileExtension()
{
    //dtor
}

void FileExtension::_setKey(){
    this->key = this->username;
    this->key.append(".");
    this->key.append(this->extension);
}
