#include "FilesGetHandler.h"
#include "UserMetadata.h"

FilesGetHandler::FilesGetHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
{
    //ctor
}

FilesGetHandler::~FilesGetHandler()
{
    //dtor
}

void FilesGetHandler::_handle(HttpRequest &hmsg){
    Status s;

    UserMetadata user_metadata(db);
    user_metadata.setUsername(hmsg.getCampo("username"));
    s = user_metadata.DBget();
    // ver status

    hmsg.setResponse(user_metadata.getFileTreeJson());
}
