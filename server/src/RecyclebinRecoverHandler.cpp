#include "RecyclebinRecoverHandler.h"
#include "UserMetadata.h"

RecyclebinRecoverHandler::RecyclebinRecoverHandler(Database* db , TokenAuthenticator* a) : EventHandlerChecksAuthentication(db,a)
{
    //ctor
}

void RecyclebinRecoverHandler::_handle(HttpRequest &hmsg){
    std::string my_username = hmsg.getUsername();
    if (my_username == "" ) return;
    std::string filename = hmsg.getUriStringParsedByIndex(3);
    if (filename == "") return;

    UserMetadata user_metadata(db);
    user_metadata.setUsername(my_username);

    Status s = user_metadata.DBget();

    if (!s.ok()){
        hmsg.setResponse(s,"No se encontro el usuario");
    } else{
        if(user_metadata.recoverFileRecycleBin(filename)){
            hmsg.setResponse(Status::OK(), "El archivo ha sido recuperado satisfactoriamente");
        }else{
            hmsg.setResponse(Status::NotFound(), "No se encontro el archivo");
        }
    }
}

RecyclebinRecoverHandler::~RecyclebinRecoverHandler()
{
    //dtor
}

