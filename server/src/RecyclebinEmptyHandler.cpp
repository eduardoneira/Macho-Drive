#include "RecyclebinEmptyHandler.h"
#include "UserMetadata.h"
#include "FileData.h"

RecyclebinEmptyHandler::RecyclebinEmptyHandler(Database* db , TokenAuthenticator* a) : EventHandlerChecksAuthentication(db,a)
{
    //ctor
}

/*
    Este handler borra todos los archivos en la papelera
*/
void RecyclebinEmptyHandler::_handle(HttpRequest &hmsg){
    Status s;
    std::string username = hmsg.getUsername();
    if(username == "") return;

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    s = user_metadata.DBget();

    if (!s.ok()) {
        hmsg.setResponse(s);
        return;
    }

    FileData file_data(db);
    file_data.setOwnerUsername(username);
    std::vector<std::string> bin = user_metadata.getAllFilesBin();

    //DB ERASE : hacer uno que deletee solo el personal
    for(std::vector<std::string>::iterator it = bin.begin(); it != bin.end(); ++it){
        file_data.setFilename(*it);
        s = file_data.DBdelete_file();
        if (!s.ok()) break;
    }

    hmsg.setResponse(s);

}

RecyclebinEmptyHandler::~RecyclebinEmptyHandler()
{
    //dtor
}
