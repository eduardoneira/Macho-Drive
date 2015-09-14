#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mongoose.h"
#include <iostream>
#include "Server.h"

/*static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_AUTH: return MG_TRUE;
    case MG_REQUEST:
			std::cout << "Recibio request" << std::endl;
			if(!strcmp(conn->request_method, "GET")){
				if(conn->uri[strlen(conn->uri)-1] == '/'){
					// get collection
					mg_printf_data(conn, "GET de coleccion: %s", conn->uri);
					mg_send_data(conn, "", 0);
					return MG_TRUE;
				} else {
					// get element
					mg_printf_data(conn, "GET de elemento: %s", conn->uri);
					mg_send_data(conn, "", 0);
					return MG_TRUE;
				}
			} else
			if(!strcmp(conn->request_method, "PUT")){
				if(conn->uri[strlen(conn->uri)-1] == '/'){
					// get collection
					mg_printf_data(conn, "PUT de coleccion: %s", conn->uri);
					return MG_TRUE;
				} else {
					// get element
					mg_printf_data(conn, "PUT de elemento: %s", conn->uri);
					return MG_TRUE;
				}
			} else
			if(!strcmp(conn->request_method, "POST")){
				if(conn->uri[strlen(conn->uri)-1] == '/'){
					// get collection
					mg_printf_data(conn, "POST de coleccion: %s", conn->uri);
					return MG_TRUE;
				} else {
					// get element
					mg_printf_data(conn, "POST de elemento: %s", conn->uri);
					return MG_TRUE;
				}
			} else
			if(!strcmp(conn->request_method, "DELETE")){
				if(conn->uri[strlen(conn->uri)-1] == '/'){
					// get collection
					mg_printf_data(conn, "POST de coleccion: %s", conn->uri);
					return MG_TRUE;
				} else {
					// get element
					mg_printf_data(conn, "POST de elemento: %s", conn->uri);
					return MG_TRUE;
				}
			} else {
				// metodo invalido
				mg_printf_data(conn, "INVALID METHOD");
				return MG_TRUE;
			}
    default: return MG_FALSE;
  }
}*/

int main(void) {
  //struct mg_server *server;

  Server server;

  // Create and configure the server
  //server = mg_create_server(NULL, ev_handler);
  //mg_set_option(server, "listening_port", "8000");

  server.createListeningConnection("8000");

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
