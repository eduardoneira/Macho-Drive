#include "FileQuery.h"
#include "User.h"
#include "UserMetadata.h"
#include "FileData.h"
#include "JsonSerializer.h"

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

void FileQuery::search_files(){

    //seteo el user_metadata
    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    Status s = user_metadata.DBget();
    //Si no se especifica ningun word, supongo q se devuelve todo
    if (this->word == ""){
        this->valid_files=user_metadata.getAll_files();
    }

    //Checkeo que tipo de metadato me pidieron buscar y le pregunto a metadata
    //Medio hardcodeo, despues revisar
    if (this->metadata == "NAME"){
        this->valid_files=user_metadata.search_files_by_name(this->word);
    }
    else if (this->metadata == "OWNER"){
        this->valid_files=user_metadata.search_files_by_owner(this->word);
    }
    else if (this->metadata == "TAG"){
        this->valid_files=user_metadata.search_files_by_tag(this->word);
    }
    else if(this->metadata == "EXTENSION"){
        this->valid_files=user_metadata.search_files_by_extension(this->word);
    }

   /* for (std::vector<std::string>::iterator it = filenames->begin(); it != filenames->end(); ++it){
        //recorrer mis archivos
    }*/
}

std::string FileQuery::getResponse(){
    JsonSerializer serializer;

    std::string json_to_return = "";
    serializer.addValueToObjectList(json_to_return,"username",this->username);
    serializer.addValueToObjectList(json_to_return,"word_searched",this->word);
    serializer.addValueToObjectList(json_to_return,"metadata_field",this->metadata);

    std::string search_result = "";
    serializer.turnVectorToArray(valid_files,"search_result",search_result);
    serializer.joinValueIntoList(json_to_return,search_result);

    serializer.turnObjectListToObject(json_to_return);
    return json_to_return;
}

FileQuery::~FileQuery()
{
    //dtor
}
