#include "RecyclebinEmptyHandler.h"

RecyclebinEmptyHandler::RecyclebinEmptyHandler(Database* db , TokenAuthenticator* a) : EventHandlerChecksAuthentication(db,a)
{
    //ctor
}

RecyclebinEmptyHandler::~RecyclebinEmptyHandler()
{
    //dtor
}
