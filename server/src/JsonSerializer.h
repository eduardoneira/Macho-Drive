#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include "json/json.h"
#include <unordered_map>

using namespace Json;

//!Clase que se encarga de tratar los archivos en formato json.
/*!Tiene diversas funciones para transformar los distintos formatos que
pueden tomar las variables del json que vamos a utilizar.
*/
class JsonSerializer
{
    public:
        JsonSerializer();
        virtual ~JsonSerializer();

        //!Funcion que pasa de lista de objetos a objeto.
        /*!Toma una lista tipo: '"nombre1" : "pedro", "nombre2" : "juan"' y la convierte en lista en '{ "nombre1" : "pedro", "nombre2" : "juan" }'.
        */
        // toma una @lista tipo: '"nombre1" : "pedro", "nombre2" : "juan"' y la convierte en @lista en '{ "nombre1" : "pedro", "nombre2" : "juan" }'
        // (en realidad solo agrega los {}, pero la idea es usarla asi)
        void turnObjectListToObject(std::string &lista);
        //!Funcion que pasa de lista de objetos a objeto bajo un nombre.
        /*!Toma una lista tipo: '"nombre1" : "pedro", "nombre2" : "juan"' y la convierte en lista en '"@name" : { "nombre1" : "pedro", "nombre2" : "juan" }'.
        */
        // toma una @lista tipo: '"nombre1" : "pedro", "nombre2" : "juan"' y la convierte en @lista en '"@name" : { "nombre1" : "pedro", "nombre2" : "juan" }'
        // (en realidad solo agrega los {}, pero la idea es usarla asi)
        void turnObjectListToObject(std::string &lista, std::string name);

        //!Funcion que pasa de lista de arrays a un array.
        /*!toma una lista tipo: '"pedro", "juan"' y la convierte en lista en '[ "pedro", "juan" ]'.
        */
        // toma una @lista tipo: '"pedro", "juan"' y la convierte en @lista en '[ "pedro", "juan" ]'
        // (en realidad solo agrega los () pero la idea es usarla asi)
        void turnArrayListToArray(std::string &lista);

        //!Funcion que pasa de lista de arrays a un array bajo un nombre.
        /*!toma una lista tipo: '"pedro", "juan"' y la convierte en lista en '"@name" : [ "pedro", "juan" ]'.
        */
        // toma una @lista tipo: '"pedro", "juan"' y la convierte en @lista en '"@name" : [ "pedro", "juan" ]'
        // (en realidad solo agrega los () pero la idea es usarla asi)
        void turnArrayListToArray(std::string &lista, std::string name);

        //!Funcion que agrega un valor a una lista de objetos.
        /*!toma una lista tipo: '"nombre1" : "pedro", "nombre2" : "juan"' y un value tipo: '"nombre3" : "jose"'
        y lo transforma en lista en  '"nombre1" : "pedro", "nombre2" : "juan", "nombre3" : "jose"'.
        */
        // toma una @lista tipo: '"nombre1" : "pedro", "nombre2" : "juan"' y un @value tipo: '"nombre3" : "jose"'
        // y lo transforma en @lista en  '"nombre1" : "pedro", "nombre2" : "juan", "nombre3" : "jose"'
        void addValueToObjectList(std::string &lista, std::string name, std::string value);

        //!Funcion que agrega un valor a un objeto.
        /*!toma &obj de la forma { "" : "", "" : "" } y le agrega un campo "name" : "value".
        */
        // toma &obj de la forma { "" : "", "" : "" } y le agrega un campo "name" : "value"
        void addValueToObject(std::string &obj, std::string name, std::string value);

        //!Funcion que agrega un valor a una lista de arrays.
        /*!Toma una lista tipo: '"pedro", "juan"' y un value tipo: '"jose"'
        y lo transforma en lista en  '"pedro", "juan", "jose"'.
        */
        // toma una @lista tipo: '"pedro", "juan"' y un @value tipo: '"jose"'
        // y lo transforma en @lista en  '"pedro", "juan", "jose"'
        void addValueToArrayList(std::string &lista, std::string value);

        //!Funcion que une un valor con una lista.
        /*!Toma una lista y un value y los une en lista tipo: 'lista, value'.
        */
        // toma una @lista y un @value y los une en @lista tipo: '@lista, @value'
        void joinValueIntoList(std::string &lista, std::string value);

        //!Funcion que agrega un array u objeto a una lista.
        /*!Toma una lista y un value tipo: '[ "", "", ""]' o tipo ''{ "", "", ""}
        y los une en lista tipo: 'lista, "name" : value'.
        */
        // toma una @lista y un @value tipo: '[ "", "", ""]' o tipo ''{ "", "", ""}
        // y los une en @lista tipo: '@lista, "@name" : @value'
        void addArrayOrObjectToList(std::string &lista, std::string name, std::string value);

        //!Funcion que agrega un numero a una lista.
        /*!Lo mismo que addValueToObjectList pero sin agregarle comillas al numero (si al nombre).
        */
        // lo mismo que addValueToObjectList pero sin agregarle comillas al numero (si al nombre)
        void addNumberToList(std::string &lista, std::string name, int number);

        //!Funcion que agrega un booleano a una lista.
        /*!Lo mismo que addValueToObjectList pero sin agregarle comillas al valor 'true' o 'false' (si al nombre).
        */
        // lo mismo que addValueToObjectList pero sin agregarle comillas al valor 'true' o 'false' (si al nombre)
        void addBoolToList(std::string &lista, std::string name, bool boolean);

        //!Funcion que transforma un vector en un array.
        /*!Recibe vec, itera por sus elementos y arma un array de la forma: [ "elem1", "elem2", "elem3" ].
        */
        // recibe @vec, itera por sus elementos y arma un array de la forma: [ "elem1", "elem2", "elem3" ]
        void turnVectorToArray(std::vector<std::string>& vec, std::string name, std::string &json);

        //!Funcion que transforma un vector de a pares en un objeto.
        /*!Recibe vec, itera por sus elementos y arma un array de la forma: { "nombre1" : "elem1", "nombre2" : "elem2", "nombre3" : "elem3" }.
        */
        // recibe @vec, itera por sus elementos y arma un array de la forma: { "nombre1" : "elem1", "nombre2" : "elem2", "nombre3" : "elem3" }
        void turnPairsVectorToObject(std::vector< std::pair<std::string, std::string> >& vec, std::string name, std::string &json);

        //!Funcion que transforma un mapa en un objeto.
        /*!Recibe mapa, itera por sus elementos y arma un obj de la forma: { "key1" : "elem1", "key2" : "elem2", "key3" : "elem3" }.
        */
        // recibe @mapa, itera por sus elementos y arma un obj de la forma: { "key1" : "elem1", "key2" : "elem2", "key3" : "elem3" }
        void turnMapToObj(std::unordered_map<std::string, std::string>& mapa, std::string name, std::string &json);

        //!Funcion get para un objeto.
        /*!Hace el get de JsonCpp en value (debe ser object), y guarda el resultado como value y string en val y str_val
        si era una hoja (es decir un valor tipo "nombre" : "pedro") en str_val se guarda: 'pedro' (sin comillas)
        si no encuentra la clave, devuelve default_val
        */
        // hace el get de JsonCpp en @value (debe ser object), y guarda el resultado como value y string en @val y @str_val
        // si era una hoja (es decir un valor tipo "nombre" : "pedro") en @str_val se guarda: 'pedro' (sin comillas)
        // si no encuentra la clave, devuelve @default_val
        static std::string get(Value value, std::string key, std::string default_val, Value &val, std::string &str_val);

        //!Funcion get para un array.
        /*!Hace el get de JsonCpp en value (debe ser array), y guarda el resultado como value y string en val y str_val
        si era una hoja (es decir un valor tipo "nombre" : "pedro") en str_val se guarda: 'pedro' (sin comillas)
        si no encuentra el indice, devuelve default_val.
        */
        // hace el get de JsonCpp en @value (debe ser array), y guarda el resultado como value y string en @val y @str_val
        // si era una hoja (es decir un valor tipo "nombre" : "pedro") en @str_val se guarda: 'pedro' (sin comillas)
        // si no encuentra el indice, devuelve @default_val
        static std::string get(Value value, int key, std::string default_val, Value &val, std::string &str_val);

        //!Funcion que saca las comillas.
        /*!Saca las "" de los valores que se agarran de jsoncpp.
        */
        // saca las "" de los valores que se agarran de jsoncpp
        static std::string removeBegAndEndQuotes(std::string);

    protected:
    private:
};

#endif // JSONSERIALIZER_H
