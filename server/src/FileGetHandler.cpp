#include "FileGetHandler.h"
#include "FileData.h"
#include "Util.h"

FileGetHandler::FileGetHandler(Database *db) : EventHandler(db)
{
    //ctor
}

FileGetHandler::~FileGetHandler()
{
    //dtor
}

void FileGetHandler::handle(HttpRequest &hmsg){
    FileData file_data;
    file_data.setOwnerUsername(hmsg.getCampo("owner_username"));
    file_data.setFilename(hmsg.getCampo("filename"));

    Status s = this->db->get(file_data);
    hmsg.setResponse(file_data.getValueToString());
}
