#include "UserMetadata.h"
#include "JsonSerializer.h"
#include <string>
#include "json/json.h"
#include "Database.h"
#include "FileData.h"
#include "Util.h"
#include "Logger.h"

#define _10GB 10485760

using namespace Json;

UserMetadata::UserMetadata(Database* db, DatabaseWriteBatch* dbbatch) : DBElement(db, dbbatch), join_date(""), cuota_actual(0), cuota_max(_10GB), email(""), ultima_ubicacion("")
{
   this->name="";
   this->email="";
   this->picture="";
   this->ultima_ubicacion="";
}

UserMetadata::~UserMetadata()
{
    //dtor
}

void UserMetadata::addMyFile(std::string name){
    if(std::find(my_files.begin(), my_files.end(), name) == my_files.end())
        this->my_files.push_back(name);
}

void UserMetadata::removeMyFile(std::string name){
    this->recycle_bin.erase(std::remove(recycle_bin.begin(), recycle_bin.end(), name), recycle_bin.end());
}

void UserMetadata::addRecycleBinFile(std::string name){
    if(std::find(recycle_bin.begin(), recycle_bin.end(), name) == recycle_bin.end())
        this->recycle_bin.push_back(name);
}

void UserMetadata::addSharedFile(std::string name, std::string user){
    if(std::find(shared_files.begin(), shared_files.end(), std::make_pair(user, name)) == shared_files.end())
        this->shared_files.push_back(std::make_pair(user, name));
}

void UserMetadata::removeSharedFile(std::string name, std::string user){
    shared_files.erase(std::remove(shared_files.begin(), shared_files.end(), std::make_pair(user, name)), shared_files.end());
}

void UserMetadata::_setKey(){
    this->key = this->username;
    this->key.append(".usertoken");
}

void UserMetadata::_setValueVars(){
    Reader reader;
    Value json_value;
    reader.parse(this->value, json_value);

    setJoinDate(JsonSerializer::removeBegAndEndQuotes(json_value["join_date"].toStyledString()));
    changeEmail(JsonSerializer::removeBegAndEndQuotes(json_value["email"].toStyledString()));
    std::string tmp_cuota_max = JsonSerializer::removeBegAndEndQuotes(json_value["cuota_max"].toStyledString());
    //if(!is_number(tmp_cuota_max)) setCuotaMax(0);
    /*else*/ setCuotaMax(std::stoi(tmp_cuota_max));
    std::string tmp_cuota_actual = JsonSerializer::removeBegAndEndQuotes(json_value["cuota_actual"].toStyledString());
    //if(!is_number(tmp_cuota_actual)) setCuotaMax(0);
    /*else*/ setCuotaActual(std::stoi(tmp_cuota_actual));
    changeUltimaUbicacion(JsonSerializer::removeBegAndEndQuotes(json_value["ultima_ubicacion"].toStyledString()));
    changeName(JsonSerializer::removeBegAndEndQuotes(json_value["name"].toStyledString()));
    changePic(JsonSerializer::removeBegAndEndQuotes(json_value["picture"].toStyledString()));

    my_files.clear();
    for(ValueIterator it = json_value["my_file_tokens"].begin(); it != json_value["my_file_tokens"].end(); ++it){
        addMyFile(JsonSerializer::removeBegAndEndQuotes((*it).asString()));
    }
    shared_files.clear();
    for(ValueIterator it = json_value["shared_file_tokens"].begin(); it != json_value["shared_file_tokens"].end(); ++it){
        addSharedFile(JsonSerializer::removeBegAndEndQuotes((*it).asString()), JsonSerializer::removeBegAndEndQuotes((it.key()).asString()));
    }

    recycle_bin.clear();
        for(ValueIterator it = json_value["recycle_bin_tokens"].begin(); it != json_value["recycle_bin_tokens"].end(); ++it){
         addRecycleBinFile(JsonSerializer::removeBegAndEndQuotes((*it).asString()));
    }


}

void UserMetadata::_setValue(){
    JsonSerializer serializer;

    std::string array_my_file_tokens = "";
    serializer.turnVectorToArray(my_files, "my_file_tokens", array_my_file_tokens);
    std::string array_shared_file_tokens = "";
    serializer.turnPairsVectorToObject(shared_files, "shared_file_tokens", array_shared_file_tokens);
    std::string val_date_joined = "";
    serializer.addValueToObjectList(val_date_joined, "join_date", join_date);
    std::string val_email = "";
    serializer.addValueToObjectList(val_email, "email", email);
    std::string val_cuota_max = "";
    serializer.addValueToObjectList(val_cuota_max, "cuota_max", std::to_string(cuota_max));
    std::string val_cuota_actual = "";
    serializer.addValueToObjectList(val_cuota_actual, "cuota_actual", std::to_string(cuota_actual));
    std::string val_ultima_ubicacion = "";
    serializer.addValueToObjectList(val_ultima_ubicacion, "ultima_ubicacion", ultima_ubicacion);
    std::string array_recycle_bin_tokens = "";
    serializer.turnVectorToArray(recycle_bin,"recycle_bin_tokens",array_recycle_bin_tokens);
    std::string val_name = "";
    serializer.addValueToObjectList(val_name,"name",this->name);
    std::string val_pic = "";
    serializer.addValueToObjectList(val_pic,"picture",this->picture);

    std::string val_json = "";
    serializer.joinValueIntoList(val_json, array_my_file_tokens);
    serializer.joinValueIntoList(val_json, array_shared_file_tokens);
    serializer.joinValueIntoList(val_json, val_date_joined);
    serializer.joinValueIntoList(val_json, val_email);
    serializer.joinValueIntoList(val_json, val_cuota_actual);
    serializer.joinValueIntoList(val_json, val_cuota_max);
    serializer.joinValueIntoList(val_json, val_ultima_ubicacion);
    serializer.joinValueIntoList(val_json,array_recycle_bin_tokens);
    serializer.joinValueIntoList(val_json,val_name);
    serializer.joinValueIntoList(val_json,val_pic);
    serializer.turnObjectListToObject(val_json);
    this->value = val_json;
}


Status UserMetadata::DBchange_shared_filename(std::string old_filename, std::string new_filename){
    Status s;

    s = this->DBget();
    if(!s.ok()) return s;

    for(int i = 0; i < shared_files.size(); ++i){
        if(shared_files[i].second == old_filename){
            shared_files[i].second = new_filename;
            break;
        }
    }

    s = this->put();
    return s;
}

Status UserMetadata::DBchange_my_filename(std::string old_filename, std::string new_filename){
    Status s;

    s = this->DBget();
    if(!s.ok()) return s;

    for(int i = 0; i < my_files.size(); ++i){
        if(my_files[i] == old_filename){
            my_files[i] = new_filename;
            break;
        }
    }

    s = this->put();
    return s;
}

std::string UserMetadata::getFileTreeJson(){
    JsonSerializer serializer;

    std::string array_my_file_tokens = "";
    serializer.turnVectorToArray(my_files, "my_file_tokens", array_my_file_tokens);
    std::string array_shared_file_tokens = "";
    serializer.turnPairsVectorToObject(shared_files, "shared_file_tokens", array_shared_file_tokens);
    std::string val_json = "";
    serializer.joinValueIntoList(val_json, array_my_file_tokens);
    serializer.joinValueIntoList(val_json, array_shared_file_tokens);
    serializer.turnObjectListToObject(val_json);
    this->value = val_json;
    return val_json;
}

// asumo que me dieron el username correcto
Status UserMetadata::DBerase(){
    Status s;

    s = this->DBget();
    if(!s.ok()){
        return s;
    }

    for(std::vector<std::string>::iterator it = my_files.begin(); it != my_files.end(); ++it){
        FileData file_data(db, this->batch);
        file_data.setOwnerUsername(this->getUsername());
        file_data.setFilename(*it);
        s = file_data.DBdelete_file();

        //s = this->DBremove_my_file(*it, 0);
        // ver status
    }

    for(std::vector<std::string>::iterator it = recycle_bin.begin(); it != recycle_bin.end(); ++it){
        FileData file_data(db, this->batch);
        file_data.setOwnerUsername(this->getUsername());
        file_data.setFilename(*it);
        s = file_data.DBdelete_file();

        //s = this->DBremove_my_file(*it, 0);
        // ver status
    }

    for(std::vector< std::pair<std::string, std::string> >::iterator it = shared_files.begin(); it != shared_files.end(); ++it){
        s = this->DBremove_shared_file(it->first, it->second);
        // ver status
    }

    s = this->erase();
    // ver status
    return s;
}

Status UserMetadata::DBget(){
    Status s;

    s = this->get();
    if(s.IsNotFound()){
        return Status::NotFound("no se encontro el usuario pedido");
    }

    return s;
}

Status UserMetadata::DBcreate(){
    Status s;

    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Corrobora que el puntero db vaya a una base de datos",INFO);
    s = this->get();
    if(!s.IsNotFound()){
        //s = this->DBerase();
        return Status::Aborted("el usuario ya existe");
    }

    this->setJoinDate(get_date_and_time());
    s = this->put();
    // ver status
    return s;
}

Status UserMetadata::DB_move_to_bin(std::string filename){
    Status s;
    this->startBatch();
    s = this->DBget();
    if (!s.ok()) return s;
    std::vector<std::string>::iterator it = std::find(this->my_files.begin(),this->my_files.end(),filename);
    if (it != this->my_files.end()) this->my_files.erase(it);

    this->recycle_bin.push_back(filename);

    s = this->put();
    this->endBatch();
    return s;
}

Status UserMetadata::DBremove_my_file(std::string filename, double tam){
    Status s;

    this->startBatch();
    s = this->DBget();
    if(!s.ok()) return s;

    this->removeMyFile(filename);
    this->remove_from_cuota(tam);

    s = this->put();
    this->endBatch();
    // ver status

    /*FileData file_data(db);
    file_data.setOwnerUsername(this->getUsername());
    file_data.setFilename(filename);
    s = file_data.DBerase();*/
    // ver status

    return s;
}

Status UserMetadata::DBremove_shared_file(std::string user, std::string filename){
    Status s;

    s = this->get();
    if(!s.ok()) return s;
    // ver status
    this->removeSharedFile(filename, user);
    s = this->put();
    // ver status
    /*FileData file_data(db);
    file_data.setOwnerUsername(user);
    file_data.setFilename(filename);
    s = file_data.DBremoveUserWithReadPermission(this->getUsername());*/
    // ver status

    return s;
}

// no crea archivo
Status UserMetadata::DBadd_my_file(std::string filename/*, double file_size, std::string u*/){
    Status s;

    s = this->get();
    // ver status
    this->addMyFile(filename);
    //this->add_to_cuota(file_size);
    //this->changeUltimaUbicacion(u);
    s = this->put();
    // ver status
    return s;
}

Status UserMetadata::DBhas_enough_cuota(double file_size, bool &result){
    Status s;
    result = false;

    s = this->DBget();
    if(s.ok()){
        result = this->cuota_actual + file_size <= this->cuota_max;
        return s;
    }

    return s;
}

// no modifica archivo
Status UserMetadata::DBadd_shared_file(std::string user, std::string filename){
    Status s;

    s = this->get();
    if(!s.ok()) return s;
    // ver status
    this->addSharedFile(filename, user);
    s = this->put();
    // ver status
    return s;
}

Status UserMetadata::DBsetCuotaMax(double c){
    Status s;

    s = this->get();
    if(!s.ok()) return s;

    this->startBatch();

    this->setCuotaMax(c);
    s = this->put();
    // ver status
    return this->endBatch();
}

Status UserMetadata::DBchange_email(std::string n_email){
    Status s;

    s = this->get();
    if(!s.ok())
        return s;

    this->startBatch();

    this->changeEmail(n_email);
    s = this->put();
    // ver status
    return this->endBatch();
}

Status UserMetadata::DBchange_name(std::string new_name){
    Status s;

    s = this->get();
    if(!s.ok())
        return s;

    this->startBatch();

    this->name = new_name;
    s = this->put();
    // ver status
    return this->endBatch();
}

Status UserMetadata::DBchange_profile_pic(std::string new_pic){
    Status s;

    s = this->get();
    if(!s.ok())
        return s;

    this->startBatch();

    this->picture = new_pic;
    s = this->put();
    // ver status
    return this->endBatch();
}

Status UserMetadata::DBchange_last_place(std::string new_place){
    Status s;

    s = this->get();
    if(!s.ok())
        return s;

    this->startBatch();

    this->ultima_ubicacion = new_place;
    s = this->put();
    // ver status
    return this->endBatch();
}

Status UserMetadata::DBchange_cuota_max(double n_cuota_max){
    Status s;

    s = this->get();
    // ver status
    this->setCuotaMax(n_cuota_max);
    s = this->put();
    // ver status
    return s;
}

Status UserMetadata::DBchange_ultima_ubicacion(std::string u){
    Status s;

    s = this->get();
    // ver status
    this->changeUltimaUbicacion(u);
    s = this->put();
    // ver status
    return s;
}

bool UserMetadata::DBisMyFile(std::string filename){
    Status s;

    s = this->get();

    for(std::vector<std::string>::iterator it = this->my_files.begin(); it != this->my_files.end(); it++){
        if(filename == *it)
            return true;
    }

    return false;
}

std::pair<std::string, std::string> UserMetadata::DBisSharedFile(std::string filename){
    Status s;

    s = this->get();

    for(std::vector< std::pair<std::string, std::string> >::iterator it = this->shared_files.begin(); it != this->shared_files.end(); it++){
        if(filename == it->second)
            return *it;
    }

    return std::make_pair("", "");
}

Status UserMetadata::DBmodif_file(double dif_cuota){
    Status s;

    s = this->get();
    // ver status
    /*if(u.compare("") != 0){
        this->changeUltimaUbicacion(u);
    }*/
    this->add_to_cuota(dif_cuota);
    s = this->put();
    // ver status
    return s;
}

std::vector<std::string> UserMetadata::search_files_by_tag(std::string word){
    std::vector<std::string> return_files;

    //FALTA UNA BUENA REFACTORIZACION
    //Checko mis archivos
    for (std::vector<std::string>::iterator it = my_files.begin(); it != my_files.end(); ++it){
       FileData file_data(this->db, this->batch);
       file_data.setOwnerUsername(this->username);
       file_data.setFilename(*it);

       Status s = file_data.DBget_for_read(this->username);
       if (file_data.check_if_tag_belongs(word))
            return_files.push_back(*it);
    }
    //Checkeo shared_files
    for (std::vector<std::pair<std::string,std::string>>::iterator it = shared_files.begin(); it != shared_files.end(); ++it){
       FileData file_data(this->db, this->batch);
       file_data.setOwnerUsername((*it).first);
       file_data.setFilename((*it).second);

       Status s = file_data.DBget_for_read(this->username);
       if (file_data.check_if_tag_belongs(word))
            return_files.push_back((*it).second);
    }

     return return_files;
}

std::vector<std::string> UserMetadata::search_files_by_owner(std::string word){
    std::vector<std::string> return_files;

    if (word == this->username)
        return this->my_files;
    else{
         for (std::vector<std::pair<std::string,std::string>>::iterator it = shared_files.begin(); it != shared_files.end(); ++it){
            if ((*it).first == word)
                return_files.push_back((*it).second);
        }
    }

    return return_files;
}

std::vector<std::string> UserMetadata::search_files_by_name(std::string word){
    std::vector<std::string> return_files;
    for (std::vector<std::string>::iterator it = my_files.begin(); it != my_files.end(); ++it){
       if ((*it).find(word) != std::string::npos)
            return_files.push_back(*it);
    }
    //Checkeo shared_files
    for (std::vector<std::pair<std::string,std::string>>::iterator it = shared_files.begin(); it != shared_files.end(); ++it){

       if (((*it).second).find(word) != std::string::npos)
            return_files.push_back((*it).second);
    }
    return return_files;
}

std::vector<std::string> UserMetadata::search_files_by_extension(std::string word){
    std::vector<std::string> return_files;

    for (std::vector<std::string>::iterator it = my_files.begin(); it != my_files.end(); ++it){
       FileData file_data(this->db, this->batch);
       file_data.setOwnerUsername(this->username);
       file_data.setFilename(*it);

       Status s = file_data.DBget_for_read(this->username);
       if (file_data.check_extension(word))
            return_files.push_back(*it);
    }
    //Checkeo shared_files
    for (std::vector<std::pair<std::string,std::string>>::iterator it = shared_files.begin(); it != shared_files.end(); ++it){
       FileData file_data(this->db, this->batch);
       file_data.setOwnerUsername((*it).first);
       file_data.setFilename((*it).second);

       Status s = file_data.DBget_for_read(this->username);
       if (file_data.check_extension(word))
            return_files.push_back((*it).second);
    }
    return return_files;
}

std::vector<std::string> UserMetadata::getAll_files(){
    std::vector<std::string> return_files;

    return_files = my_files;
    for (std::vector<std::pair<std::string,std::string>>::iterator it = shared_files.begin(); it != shared_files.end(); ++it){
        return_files.push_back((*it).second);
    }
    return return_files;
}

std::string UserMetadata::getRecycleBin(){
    JsonSerializer serializer;

    std::string json_to_return = "";
    serializer.addValueToObjectList(json_to_return,"username",this->username);

    std::string bin = "";
    serializer.turnVectorToArray(this->recycle_bin,"files_in_bin",bin);
    serializer.joinValueIntoList(json_to_return,bin);

    serializer.turnObjectListToObject(json_to_return);
    return json_to_return;
}

bool UserMetadata::recoverFileRecycleBin(std::string filename){
    std::vector<std::string>::iterator it;

    if ((it = std::find(this->recycle_bin.begin(),this->recycle_bin.end(),filename)) != this->recycle_bin.end()){
        this->my_files.push_back(filename);
        this->recycle_bin.erase(it);
        this->put();
        return true;
    }
    return false;
}

std::string UserMetadata::getProfile(){
    JsonSerializer serializer;

    std::string json_to_return = "";
    serializer.addValueToObjectList(json_to_return,"username",this->username);
    serializer.addValueToObjectList(json_to_return,"name",this->name);
    serializer.addValueToObjectList(json_to_return,"picture",this->picture);
    serializer.addValueToObjectList(json_to_return,"email",this->email);
    serializer.addValueToObjectList(json_to_return,"place",this->ultima_ubicacion);

    serializer.turnObjectListToObject(json_to_return);
    return json_to_return;


}
