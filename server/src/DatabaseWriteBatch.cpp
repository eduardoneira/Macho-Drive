#include "DatabaseWriteBatch.h"
#include "DBElement.h"
#include "Database.h"

/*
NOTA: la idea es que en las funciones publicas de cada DBElement, lo primero que se hace es startBatch().
De ahi en mas, cada operacion que se haga no se va a 'hacer' sobre la base de datos, sino que se guarda en el batch
Al final, hacer endBatch() y devolver el status que devuelve (este es el q importa pq este es el que habla con la DB)
Como no hay merge operators, es importante asegurarse que en cada batch haya solo un put para cada clave.
Si hay mas de uno se pisan los anteriores, asi que hay que tener en cuenta que ese unico put tenga todas las modif necesarias.
*/

DatabaseWriteBatch::DatabaseWriteBatch(Database *_db)
{
    this->db = _db;
}

DatabaseWriteBatch::~DatabaseWriteBatch()
{
    //dtor
}

void DatabaseWriteBatch::Erase(DBElement *elem){
    this->keys.push_back(elem->getKey());
    this->values.push_back(elem->getValue());
    this->operations.push_back("erase");
}

void DatabaseWriteBatch::Put(DBElement *elem){
    this->keys.push_back(elem->getKey());
    this->values.push_back(elem->getValue());
    this->operations.push_back("put");
}
