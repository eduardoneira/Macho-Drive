#include "UserModifyHandler.h"
#include "UserMetadata.h"

UserModifyHandler::UserModifyHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
{
    //ctor
}

UserModifyHandler::~UserModifyHandler()
{
    //dtor
}

bool UserModifyHandler::isMyRequest(HttpRequest &hmsg){
    // PUT /users/'username' quiere decir modificar perfil de tal usuario
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS &&
        hmsg.getUriCantCampos() == 2 &&
        hmsg.getUriType() ==  HttpRequest::ELEMENT_URI &&
        hmsg.getMethod() == HttpRequest::PUT){
        return true;
    }
    return false;
}

void UserModifyHandler::_handle(HttpRequest &hmsg){
    Status s;

    std::string username = hmsg.getUsername();
    std::string email = hmsg.getCampo("email");
    std::string picture = hmsg.getCampo("picture");
    std::string place = hmsg.getCampo("place");
    std::string name = hmsg.getCampo("name");

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    // tal vez cambiar a ModifyUser como con los archivos
    s = user_metadata.DBchange_email(email);
    if (s.ok()) s = user_metadata.DBchange_profile_pic(picture);
    if (s.ok()) s = user_metadata.DBchange_last_place(place);
    if (s.ok()) s = user_metadata.DBchange_name(name);
    // ver status
    hmsg.setResponse(s);
}
