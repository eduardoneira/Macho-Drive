#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mongoose/mongoose.h"
#include <iostream>
#include "Server.h"
#include <time.h>

int main(int argc, char** argv) {
    //struct mg_server *server;

    srand(time(NULL));

    // se reciben como parametros opcionales '-Ddb_path,/home/mi_path' y '-Dcreate_if_missing,true'
    std::string db_path = "/tmp/test";
    bool create_if_missing = true;
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
        }
    }

    Server server(db_path, create_if_missing);

    // Create and configure the server
    //server = mg_create_server(NULL, ev_handler);
    //mg_set_option(server, "listening_port", "8000");

    if(!server.createListeningConnection("8000")){
        //log error de mongoose
    }

    // Serve request. Hit Ctrl-C to terminate the program

    std::cout << "Starting on port: 8000, database path: " << db_path << std::endl;
    for (;;) {
        server.poll();
    }

    // Cleanup, and free server instance
    //mg_destroy_server(&server);
    server.stop();

    return 0;
}
