#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mongoose/mongoose.h"
#include <iostream>
#include "Server.h"
#include <time.h>


int main(void) {
    //struct mg_server *server;

    srand(time(NULL));

    Server server;

    // Create and configure the server
    //server = mg_create_server(NULL, ev_handler);
    //mg_set_option(server, "listening_port", "8000");

    if(!server.createListeningConnection("8000")){
        //log error de mongoose
    }

    // Serve request. Hit Ctrl-C to terminate the program

    printf("Starting on port %s\n", "8000");
    for (;;) {
        server.poll();
    }

    // Cleanup, and free server instance
    //mg_destroy_server(&server);
    server.stop();

    return 0;
}
