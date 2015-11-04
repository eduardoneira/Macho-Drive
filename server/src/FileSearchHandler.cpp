#include "FileSearchHandler.h"
#include "JsonSerializer.h"
#include "UserMetadata.h"
#include "FileData.h"
#include "FileExtension.h"
#include "FileTag.h"
#include "FileName.h"
#include "FileQuery.h"

FileSearchHandler::FileSearchHandler(Database* db , TokenAuthenticator* a) : EventHandlerChecksAuthentication(db,a)
{
    //ctor
}

void FileSearchHandler::_handle(HttpRequest& hmsg){
    Status s;

    std::string my_username = hmsg.getUsername();
    //std::string metadata_to_search = hmsg.getCampo("metadata_to_search");
    std::string metadata_to_search = hmsg.getQueryCampo("metadata_to_search");
    //std::string word_to_search = hmsg.getCampo("word_to_search");
    std::string word_to_search = hmsg.getQueryCampo("word_to_search");

    FileQuery query(my_username,metadata_to_search,word_to_search,db);

    query.search_files();

    hmsg.setResponse(query.getResponse());

    //Falta devolver una Respuestas HTTP


}

FileSearchHandler::~FileSearchHandler()
{
    //dtor
}
