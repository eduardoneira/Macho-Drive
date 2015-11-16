#include "DBElementMock.h"

DBElementMock::DBElementMock(Database* db, std::string c, std::string v, DatabaseWriteBatch* dbbatch) : DBElement(db, dbbatch)
{
    this->setMockClave(c);
    this->setMockValue(v);
}

DBElementMock::~DBElementMock()
{
    //dtor
}
