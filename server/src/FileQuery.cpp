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

std::vector<std::string> FileQuery::search_files(){
    std::vector<std::string> valid_files;

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    Status s = user_metadata.DBget();
    //Si no se especifica ningun word, supongo q se devuelve todo
    if (this->word == ""){
        return user_metadata.getAll_files();
    }

    //Checkeo que tipo de metadato me pidieron buscar y le pregunto a metadata
    //Medio hardcodeo, despues revisar
    if (this->metadata == "NAME"){
        return user_metadata.search_files_by_name(this->word);
    }
    else if (this->metadata == "OWNER"){
        return user_metadata.search_files_by_owner(this->word);
    }
    else if (this->metadata == "TAG"){
        return user_metadata.search_files_by_tag(this->word);
    }
    else if(this->metadata == "EXTENSION"){
        return user_metadata.search_files_by_extension(this->word);
    }

   /* for (std::vector<std::string>::iterator it = filenames->begin(); it != filenames->end(); ++it){
        //recorrer mis archivos
    }*/

    return valid_files;
}

FileQuery::~FileQuery()
{
    //dtor
}
