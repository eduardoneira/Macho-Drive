#ifndef TOKENAUTHENTICATOR_H
#define TOKENAUTHENTICATOR_H

#include <string>
#include <unordered_map>

//!Clase que se encarga de manejar tokens.
/*!Esta clase crea, valida y destruye tokens empleando una clase map.
*/
class TokenAuthenticator
{
    public:

        //!Funcion que inicializa la clase.
        TokenAuthenticator();
        virtual ~TokenAuthenticator();

        //!Funcion que valida tokens.
        /*!Esta función recibe un user y un token y busca en su map si se encuentran.
        */
        bool isValidToken(std::string user, std::string token);

        //!Funcion que agrega tokens.
        /*!Recibe un user y devuelve un token.
        */
        std::string createToken(std::string user);

        //!Funcion que remueve tokens.
        /*!Recibe el user a borrar y lo elimina de su map.
        */
        bool removeToken(std::string user);

    protected:
    private:

        //!Variable que guarda tokens.
        /*!Almacena en un unordered_map los tokens validos.
        */
        std::unordered_map<std::string, std::string> active_tokens;
};

#endif // TOKENAUTHENTICATOR_H
