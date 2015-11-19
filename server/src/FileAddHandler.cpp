#include "FileAddHandler.h"
#include "FileData.h"
#include "JsonSerializer.h"
#include "Util.h"
#include "UserMetadata.h"
#include "Logger.h"

FileAddHandler::FileAddHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
{
    //ctor
}

FileAddHandler::~FileAddHandler()
{
    //dtor
}

bool FileAddHandler::isMyRequest(HttpRequest &hmsg){
    // POST /files/'username'/ quiere decir subir archivo de tal usuario
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo FileAdd",INFO);
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES &&
        hmsg.getUriCantCampos() == 2 &&
        hmsg.getUriType() ==  HttpRequest::COLLECTION_URI &&
        hmsg.getMethod() == HttpRequest::POST){
        log->Log("Confirma que es un Handler tipo FileAdd",INFO);
        return true;
    }
    return false;
}

void FileAddHandler::_handle(HttpRequest &hmsg){
    Status s;

    Server_Logger* log = Server_Logger::getInstance();
    std::string filename = hmsg.getCampo("filename");
    log->Log("El campo recibido por filename es : "+filename,DEBUG);
    if(filename == "") return;
    std::string owner_username = hmsg.getUsername();
    log->Log("El campo recibido por owner username es : "+owner_username,DEBUG);
    if(owner_username == "") return;
    std::string ubicacion = hmsg.getCampo("ubicacion");
    std::string content = hmsg.getCampo("content");
    log->Log("El campo recibido por content es : "+content,DEBUG);
    if(content == "") return;

    FileData file_data(db);
    file_data.setFilename(filename);
    file_data.setOwnerUsername(owner_username);

    s = file_data.DBcreate(content, ubicacion);
    if(!s.ok()){
        hmsg.setResponse(s);
        return;
    }

    // esto va aca, o directo cuando creas un archivo no tiene nada de esto y se agrega con modificaciones?

    std::string tag = "tags";
    std::vector<std::string> tags;
    for(int i = 0;; ++i){
        tag = hmsg.getCampoDeArray("tags", i);
        if(tag == "")
            break;
        //s = file_data.DBaddTag(tag);
        // ver status
        tags.push_back(tag);
    }

    std::string user_with_read_perm = "user_read_perm";
    std::vector<std::string> users_with_read_perm;
    for(int i = 0;; ++i){
        user_with_read_perm = hmsg.getCampoDeArray("users_with_read_permission", i);
        if(user_with_read_perm == "")
            break;
        //s = file_data.DBaddUserWithReadPermission(user_with_read_perm);
        // ver status
        users_with_read_perm.push_back(user_with_read_perm);
    }

    std::string user_with_write_perm = "user_write_perm";
    std::vector<std::string> users_with_write_perm;
    for(int i = 0;; ++i){
        user_with_write_perm = hmsg.getCampoDeArray("users_with_write_permission", i);
        if(user_with_write_perm == "")
            break;
        //s = file_data.DBaddUserWithWritePermission(user_with_write_perm);
        // ver status
        users_with_write_perm.push_back(user_with_write_perm);
    }

    std::vector<std::string> empty;
    std::vector<int> empty2;
    s = file_data.DBmodify(owner_username, "", "", "", users_with_read_perm, empty, users_with_write_perm, empty, tags, empty, empty2);

    hmsg.setResponse(s);
}
