#include "UserMetadata.h"
#include "JsonSerializer.h"
#include <string>
#include "json/json.h"
#include "Database.h"
#include "FileData.h"
#include "Util.h"

#define _10GB 10485760

using namespace Json;

UserMetadata::UserMetadata(Database* db) : DBElement(db), join_date(""), cuota_actual(0), cuota_max(_10GB), email(""), ultima_ubicacion("")
{
    //ctor
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
    my_files.erase(std::remove(my_files.begin(), my_files.end(), name), my_files.end());
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
    setCuotaMax(std::stoi(JsonSerializer::removeBegAndEndQuotes(json_value["cuota_max"].toStyledString())));
    setCuotaActual(std::stoi(JsonSerializer::removeBegAndEndQuotes(json_value["cuota_actual"].toStyledString())));
    changeUltimaUbicacion(JsonSerializer::removeBegAndEndQuotes(json_value["ultima_ubicacion"].toStyledString()));

    my_files.clear();
    for(ValueIterator it = json_value["my_file_tokens"].begin(); it != json_value["my_file_tokens"].end(); ++it){
        addMyFile(JsonSerializer::removeBegAndEndQuotes((*it).asString()));
    }
    shared_files.clear();
    for(ValueIterator it = json_value["shared_file_tokens"].begin(); it != json_value["shared_file_tokens"].end(); ++it){
        addSharedFile(JsonSerializer::removeBegAndEndQuotes((*it).asString()), JsonSerializer::removeBegAndEndQuotes((it.key()).asString()));
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

    std::string val_json = "";
    serializer.joinValueIntoList(val_json, array_my_file_tokens);
    serializer.joinValueIntoList(val_json, array_shared_file_tokens);
    serializer.joinValueIntoList(val_json, val_date_joined);
    serializer.joinValueIntoList(val_json, val_email);
    serializer.joinValueIntoList(val_json, val_cuota_actual);
    serializer.joinValueIntoList(val_json, val_cuota_max);
    serializer.joinValueIntoList(val_json, val_ultima_ubicacion);
    serializer.turnObjectListToObject(val_json);
    this->value = val_json;
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
}

// asumo que me dieron el username correcto
Status UserMetadata::DBerase(){
    Status s;

    s = this->DBget();
    if(!s.ok()){
        return s;
    }

    for(std::vector<std::string>::iterator it = my_files.begin(); it != my_files.end(); ++it){
        FileData file_data(db);
        file_data.setOwnerUsername(this->getUsername());
        file_data.setFilename(*it);
        s = file_data.DBerase();

        //s = this->DBremove_my_file(*it, 0);
        // ver status
    }

    for(std::vector< std::pair<std::string, std::string> >::iterator it = shared_files.begin(); it != shared_files.end(); ++it){
        s = this->DBremove_shared_file(it->first, it->second);
        // ver status
    }

    s = this->db->erase(*this);
    // ver status
    return s;
}

Status UserMetadata::DBget(){
    Status s;

    s = this->db->get(*this);
    if(s.IsNotFound()){
        return Status::NotFound("no se encontro el usuario pedido");
    }

    return s;
}

Status UserMetadata::DBcreate(){
    Status s;

    s = this->db->get(*this);
    if(!s.IsNotFound()){
        s = this->DBerase();
        return Status::Aborted("el usuario ya existe");
    }

    this->setJoinDate(get_date_and_time());
    s = this->db->put(*this);
    // ver status
    return s;
}

Status UserMetadata::DBremove_my_file(std::string filename, double tam){
    Status s;

    s = this->DBget();
    if(!s.ok()) return s;

    this->removeMyFile(filename);
    this->remove_from_cuota(tam);
    s = this->db->put(*this);
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

    s = this->db->get(*this);
    // ver status
    this->removeSharedFile(filename, user);
    s = this->db->put(*this);
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

    s = this->db->get(*this);
    // ver status
    this->addMyFile(filename);
    //this->add_to_cuota(file_size);
    //this->changeUltimaUbicacion(u);
    s = this->db->put(*this);
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

    s = this->db->get(*this);
    // ver status
    this->addSharedFile(filename, user);
    s = this->db->put(*this);
    // ver status
    return s;
}

Status UserMetadata::DBchange_email(std::string n_email){
    Status s;

    s = this->db->get(*this);
    // ver status
    this->changeEmail(n_email);
    s = this->db->put(*this);
    // ver status
    return s;
}

Status UserMetadata::DBchange_cuota_max(double n_cuota_max){
    Status s;

    s = this->db->get(*this);
    // ver status
    this->setCuotaMax(n_cuota_max);
    s = this->db->put(*this);
    // ver status
    return s;
}

Status UserMetadata::DBchange_ultima_ubicacion(std::string u){
    Status s;

    s = this->db->get(*this);
    // ver status
    this->changeUltimaUbicacion(u);
    s = this->db->put(*this);
    // ver status
    return s;
}

Status UserMetadata::DBmodif_file(double dif_cuota){
    Status s;

    s = this->db->get(*this);
    // ver status
    /*if(u.compare("") != 0){
        this->changeUltimaUbicacion(u);
    }*/
    this->add_to_cuota(dif_cuota);
    s = this->db->put(*this);
    // ver status
    return s;
}
