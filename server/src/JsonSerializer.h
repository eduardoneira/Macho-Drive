#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include "json/json.h"
#include <unordered_map>

using namespace Json;

class JsonSerializer
{
    public:
        JsonSerializer();
        virtual ~JsonSerializer();

        // toma una @lista tipo: '"nombre1" : "pedro", "nombre2" : "juan"' y la convierte en @lista en '{ "nombre1" : "pedro", "nombre2" : "juan" }'
        // (en realidad solo agrega los {}, pero la idea es usarla asi)
        void turnObjectListToObject(std::string &lista);
        // toma una @lista tipo: '"nombre1" : "pedro", "nombre2" : "juan"' y la convierte en @lista en '"@name" : { "nombre1" : "pedro", "nombre2" : "juan" }'
        // (en realidad solo agrega los {}, pero la idea es usarla asi)
        void turnObjectListToObject(std::string &lista, std::string name);
        // toma una @lista tipo: '"pedro", "juan"' y la convierte en @lista en '[ "pedro", "juan" ]'
        // (en realidad solo agrega los () pero la idea es usarla asi)
        void turnArrayListToArray(std::string &lista);
        // toma una @lista tipo: '"pedro", "juan"' y la convierte en @lista en '"@name" : [ "pedro", "juan" ]'
        // (en realidad solo agrega los () pero la idea es usarla asi)
        void turnArrayListToArray(std::string &lista, std::string name);
        // toma una @lista tipo: '"nombre1" : "pedro", "nombre2" : "juan"' y un @value tipo: '"nombre3" : "jose"'
        // y lo transforma en @lista en  '"nombre1" : "pedro", "nombre2" : "juan", "nombre3" : "jose"'
        void addValueToObjectList(std::string &lista, std::string name, std::string value);
        // toma &obj de la forma { "" : "", "" : "" } y le agrega un campo "name" : "value"
        void addValueToObject(std::string &obj, std::string name, std::string value);
        // toma una @lista tipo: '"pedro", "juan"' y un @value tipo: '"jose"'
        // y lo transforma en @lista en  '"pedro", "juan", "jose"'
        void addValueToArrayList(std::string &lista, std::string value);
        // toma una @lista y un @value y los une en @lista tipo: '@lista, @value'
        void joinValueIntoList(std::string &lista, std::string value);
        // toma una @lista y un @value tipo: '[ "", "", ""]' o tipo ''{ "", "", ""}
        // y los une en @lista tipo: '@lista, "@name" : @value'
        void addArrayOrObjectToList(std::string &lista, std::string name, std::string value);
        // lo mismo que addValueToObjectList pero sin agregarle comillas al numero (si al nombre)
        void addNumberToList(std::string &lista, std::string name, int number);
        // lo mismo que addValueToObjectList pero sin agregarle comillas al valor 'true' o 'false' (si al nombre)
        void addBoolToList(std::string &lista, std::string name, bool boolean);
        // recibe @vec, itera por sus elementos y arma un array de la forma: [ "elem1", "elem2", "elem3" ]
        void turnVectorToArray(std::vector<std::string>& vec, std::string name, std::string &json);
        // recibe @vec, itera por sus elementos y arma un array de la forma: { "nombre1" : "elem1", "nombre2" : "elem2", "nombre3" : "elem3" }
        void turnPairsVectorToObject(std::vector< std::pair<std::string, std::string> >& vec, std::string name, std::string &json);
        // recibe @mapa, itera por sus elementos y arma un obj de la forma: { "key1" : "elem1", "key2" : "elem2", "key3" : "elem3" }
        void turnMapToObj(std::unordered_map<std::string, std::string>& mapa, std::string name, std::string &json);
        // hace el get de JsonCpp en @value (debe ser object), y guarda el resultado como value y string en @val y @str_val
        // si era una hoja (es decir un valor tipo "nombre" : "pedro") en @str_val se guarda: 'pedro' (sin comillas)
        // si no encuentra la clave, devuelve @default_val
        static std::string get(Value value, std::string key, std::string default_val, Value &val, std::string &str_val);
        // hace el get de JsonCpp en @value (debe ser array), y guarda el resultado como value y string en @val y @str_val
        // si era una hoja (es decir un valor tipo "nombre" : "pedro") en @str_val se guarda: 'pedro' (sin comillas)
        // si no encuentra el indice, devuelve @default_val
        static std::string get(Value value, int key, std::string default_val, Value &val, std::string &str_val);
        // saca las "" de los valores que se agarran de jsoncpp
        static std::string removeBegAndEndQuotes(std::string);

    protected:
    private:
};

#endif // JSONSERIALIZER_H
