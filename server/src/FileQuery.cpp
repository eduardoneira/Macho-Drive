#include "FileQuery.h"
#include "User.h"
#include "UserMetadata.h"
#include "FileData.h"

FileQuery::FileQuery()
{
    //ctor
}

FileQuery::FileQuery(std::string user,std::string meta, std::string w, Database* database){
    this->username=user;
    this->metadata=meta;
    this->word=w;
    this->db=database;
}

Status FileQuery::init(){
    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    Status s = user_metadata.DBget();

    filenames = user_metadata.getMy_files();

    //Falta Recuperar archivos de otro

    //En vez de tenerlo yo me conviene quedarme con una copia del user_metadata y hacer los metodos ahi

    return s;
}

std::vector<std::string>* FileQuery::search_files(){
    for (std::vector<std::string>::iterator it = filenames->begin(); it != filenames->end(); ++it){
        //recorrer mis archivos
    }
}

FileQuery::~FileQuery()
{
    //dtor
}
