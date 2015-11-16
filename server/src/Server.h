#ifndef SERVER_H
#define SERVER_H

#include "mongoose/mongoose.h"
#include "HandlerManager.h"

//!Clase que maneja el server.

class Server
{
    public:
        //!Funcion que inicializa la clase.
        /*!Con el string y el bool que recibe crea un HandlerManager\
        Luego crea una estructura mg_mgr y establece active en FALSE.
        */
        Server(std::string, bool);
        virtual ~Server();

        //!Funcion que comienza el chequeo de llegadas de mensajes.
        /*!Llama a la funcion poll de mongoose.
        */
        void poll();
        //!Funcion que para el chequeo de llegadas de mensajes.
        /*!Llama a la funcion free de mongoose.
        */
        void stop();
        //!Funcion que establece la conexion para recibir mensajes.
        /*!Recibe la direccion a donde conectarse y con mongoose establece la conexion.
        */
        bool createListeningConnection(const char * addr);

    protected:

    private:
        //!Variable puntero a HandlerManager.
        HandlerManager* handlerManager;
        //!Variable estructura mg_mgr.
        struct mg_mgr* server_mgr;
        //!Variable bool.
        bool active;
        //!Variable puntero a server.
        static Server* open_server;

        //!Funcion handler que se le envia a mongoose.
        /*!Necesita ser static para que mongoose la pueda procesar, pero luego llama a handler.
        */
        static void staticHandler(struct mg_connection *nc, int ev, void* ev_data);
        //!Funcion que procesa los mensajes que le llegan.
        /*!Reconoce si los mensajes que recibe son httprequest y de ser asi los procesa.
        */
        virtual void handler(struct mg_connection *nc, int ev, void* ev_data);
};

#endif // SERVER_H
