#include "FileSearchHandler.h"
#include "JsonSerializer.h"
#include "UserMetadata.h"
#include "FileData.h"
#include "FileQuery.h"
#include <iostream>
#include "Logger.h"

FileSearchHandler::FileSearchHandler(Database* db , TokenAuthenticator* a) : EventHandlerChecksAuthentication(db,a)
{
    //ctor
}

bool FileSearchHandler::isMyRequest(HttpRequest &hmsg){
    // GET /files/'username'/search/val1/val2 busca en los archivos del usuario segun el campo val1 (tag, nombre o extension) y valor val2(.exe, test, miarch.txt, etc)
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo FileSearch",INFO);
    if (hmsg.getUriParsedByIndex(0) == HttpRequest::FILES &&
        hmsg.getUriParsedByIndex(2) == HttpRequest::SEARCH &&
        hmsg.getUriCantCampos() == 5 &&
        hmsg.getMethod() == HttpRequest::GET){
        log->Log("Confirma que es un Handler tipo FileSearch",INFO);
        return true;
    }
    return false;
}

void FileSearchHandler::_handle(HttpRequest& hmsg){
    Status s;

    Server_Logger* log = Server_Logger::getInstance();
    std::string my_username = hmsg.getUsername();
    log->Log("El campo recibido por username es : "+my_username,DEBUG);
    if(my_username == ""){
        hmsg.setResponse(Status::InvalidArgument());
        return;
    }
    //std::string metadata_to_search = hmsg.getCampo("metadata_to_search");
    std::string metadata_to_search = hmsg.getQueryCampo("metadata_to_search");
    log->Log("El campo recibido por query para metadata es : "+metadata_to_search,DEBUG);
    //std::string word_to_search = hmsg.getCampo("word_to_search");
    std::string word_to_search = hmsg.getQueryCampo("word_to_search");
    log->Log("El campo recibido por query para palabra es : "+word_to_search,DEBUG);

    FileQuery query(my_username,metadata_to_search,word_to_search,db);

    //std::cout << "meta : " <<metadata_to_search << "  " << "word : " << word_to_search << std::endl;
    query.search_files();

    // EDU FIJATE ACA, LO PUSE ASI PERO NI IDEA
    hmsg.setResponse(Status::OK(), query.getResponse());

    //Falta devolver una Respuestas HTTP


}

FileSearchHandler::~FileSearchHandler()
{
    //dtor
}
