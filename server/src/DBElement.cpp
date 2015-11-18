#include "DBElement.h"
#include "Database.h"
#include "DatabaseWriteBatch.h"

DBElement::DBElement(Database *datab, DatabaseWriteBatch* databbatch) : key(""), value(""), db(datab), batch(databbatch), synced(false)
{

}

DBElement::~DBElement()
{
    //dtor
}

void DBElement::setValue(std::string value){
    this->value = value;
    this->_setValueVars();
}

std::string DBElement::getKey(){
    this->_setKey();
    return this->key;
}

std::string DBElement::getValue(){
    this->_setValue();
    return this->value;
}

Status DBElement::put(){
    if(db == NULL){
        return Status::Aborted("No se encontro la base de datos");
    }

    Status s = Status::OK();

    if(this->batch != NULL){
        batch->Put(this);
    } else {
        s = this->db->put(*this);
    }

    return s;
}

Status DBElement::erase(){
    if(db == NULL){
        return Status::Aborted("No se encontro la base de datos");
    }

    Status s = Status::OK();

    if(this->batch != NULL){
        batch->Erase(this);
    } else {
        s = this->db->erase(*this);
    }

    return s;
}

Status DBElement::get(){
    if(db == NULL){
        return Status::Aborted("No se encontro la base de datos");
    }
    if(synced){
        return Status::OK();
    }

    Status s;
    s = this->db->get(*this);
    //if(s.ok())
    synced = true; // ver si synced trae quilombos

    return s;
}

void DBElement::startBatch(){
    this->batch = new DatabaseWriteBatch(this->db);
}

Status DBElement::endBatch(){
    Status s = this->db->writeBatch(this->batch);
    delete this->batch;
    this->batch = NULL;
    return s;
}

void DBElement::reSync(){
    this->synced = false;
}
