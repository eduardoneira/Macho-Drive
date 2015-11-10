#include "RecyclebinGetHandler.h"
#include "UserMetadata.h"

RecyclebinGetHandler::RecyclebinGetHandler(Database* db , TokenAuthenticator* a) : EventHandlerChecksAuthentication(db,a)
{
    //ctor
}

/*
En este handle se devulve los archivos que esten en la papelera de reciclaje del usuario que pidio.
*/

void RecyclebinGetHandler::_handle(HttpRequest &hmsg){

    std::string my_username = hmsg.getUsername();

    UserMetadata user_metadata(db);
    user_metadata.setUsername(my_username);

    Status s = user_metadata.DBget();

    if (!s.ok()){
        hmsg.setResponse(s,"No se encontro el usuario");
    } else{
        hmsg.setResponse(Status::OK(), user_metadata.getRecycleBin());
    }

}

RecyclebinGetHandler::~RecyclebinGetHandler()
{
    //dtor
}
