#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mongoose/mongoose.h"
#include <iostream>
#include "Server.h"
#include <time.h>
#include <Logger.h>

int main(int argc, char** argv) {
    //struct mg_server *server;

    srand(time(NULL));

    /// se reciben como parametros opcionales (lo que esta despues de la coma es como ejemplo):
    // -db_path=/home/mi_path
    // -create_if_missing=true
    // -port=8000
    // -clean=true
    std::string db_path = "/tmp/prod";
    bool create_if_missing = true;
    bool clean_on_start = false;
    std::string port = "8000";

    for(int i = 1; i < argc; i++){
        std::string arg = argv[i];
         // borro '-'
        if(arg.length() >= 1){
            arg.erase(0, 1);
        } else {
            std::cout << "el argumento: " << arg << " es invalido" << std::endl;
            continue;
        }
        // separo 'asd=dsds' en 'asd' y 'dsds'
        std::string arg_id = "";
        for(int i = 0; i < arg.length(); i++){
            if(arg[i] == '='){
                arg.erase(0, i+1);
                break;
            }
            arg_id.push_back(arg[i]);
        }
        // veo a que arg corresponde arg_id
        if(arg_id == "db_path"){
            db_path = arg;
        } else if(arg_id == "create_if_missing"){
            if(arg == "true"){
                create_if_missing = true;
            } else if(arg == "false"){
                create_if_missing = false;
            }
        } else if(arg_id == "port"){
            port = arg;
        } else if(arg_id == "clean"){
            if(arg == "true"){
                clean_on_start = true;
            } else if(arg == "false"){
                clean_on_start = false;
            }
        }
    }

    Server server(db_path, create_if_missing, clean_on_start);

    // Create and configure the server
    //server = mg_create_server(NULL, ev_handler);
    //mg_set_option(server, "listening_port", "8000");

    std::string port_completo = ":";
    port_completo.append(port);
    if(!server.createListeningConnection(port_completo.c_str())){
        //log error de mongoose
    }

    // Serve request. Hit Ctrl-C to terminate the program

    std::cout << "Starting on addr: " << port << ", database path: " << db_path << std::endl;
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Comienza el servidor en el puerto"+port+" con la db en "+db_path,INFO);
    for (;;) {
        server.poll();
    }

    // Cleanup, and free server instance
    //mg_destroy_server(&server);
    server.stop();

    return 0;
}
