#include "FilesGetHandler.h"
#include "UserMetadata.h"

FilesGetHandler::FilesGetHandler(Database *db) : EventHandler(db)
{
    //ctor
}

FilesGetHandler::~FilesGetHandler()
{
    //dtor
}

void FilesGetHandler::handle(HttpRequest &hmsg){
    UserMetadata user_metadata(db);
    user_metadata.setUsername(hmsg.getCampo("username"));
    Status s = this->db->get(user_metadata);

    hmsg.setResponse(user_metadata.getFileTreeJson());
}
