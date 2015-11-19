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
    // -Ddb_path,/home/mi_path
    // -Dcreate_if_missing,true
    // -Dport,:8000
    std::string db_path = "/tmp/prod";
    bool create_if_missing = true;
    std::string port = ":8000";

    for(int i = 1; i < argc; i++){
        std::string arg = argv[i];
         // borro '-D'
        if(arg.length() >= 2){
            arg.erase(0, 2);
        } else {
            std::cout << "el argumento: " << arg << " es invalido" << std::endl;
        }
        // separo 'asd,dsds' en 'asd' y 'dsds'
        std::string arg_id = "";
        for(int i = 0; i < arg.length(); i++){
            if(arg[i] == ','){
                arg.erase(0, i+1);
                break;
            }
            arg_id.push_back(arg[i]);
        }
        // veo a que arg corresponde arg_id
        if(arg_id == "db_path"){
            db_path = arg;
        } else if(arg_id == "create_if_missing"){ //este no lo testie
            if(arg == "true"){
                create_if_missing = true;
            } else if(arg == "false"){
                create_if_missing = false;
            }
        } else if(arg_id == "port"){
            host_addr = arg;
        }
    }

    Server server(db_path, create_if_missing);

    // Create and configure the server
    //server = mg_create_server(NULL, ev_handler);
    //mg_set_option(server, "listening_port", "8000");

    if(!server.createListeningConnection(host_addr.c_str())){
        //log error de mongoose
    }

    // Serve request. Hit Ctrl-C to terminate the program

    std::cout << "Starting on addr: " << host_addr << ", database path: " << db_path << std::endl;
    Logger* log = Logger::getInstance();
    log->Log("Comienza el servidor en el puerto 8000",INFO);
    for (;;) {
        server.poll();
    }

    // Cleanup, and free server instance
    //mg_destroy_server(&server);
    server.stop();

    return 0;
}
