// Copyright (c) 2014 Cesanta Software
// All rights reserved
//
// This example demostrates how to connect to the remote Web server,
// download data, process it and send back a reply.

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>

#include "mongoose.h"

static struct mg_server *s_server = NULL;
static const char *s_remote_addr = "localhost:8000";
std::string req_method;
std::string req_uri;
bool connection_active;
struct mg_connection *client, *orig, *server;

bool connect(){
	if ((server = mg_connect(s_server, s_remote_addr)) != NULL) {
    // Interconnect requests
    //client->connection_param = conn;
    //conn->connection_param = client;
		std::cout << "Estableciendo conexion" << std::endl;
    return true;
  } else {
    std::cout << "cannot send API request" << std::endl;
    return false;
	}
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  struct mg_connection *client, *orig;

  switch (ev) {
    case MG_AUTH:
      return MG_TRUE;

    case MG_CONNECT:
			//std::cout << "se recibio MG_CONNECT" << std::endl;
    	
			if(conn->status_code != 0){
				std::cout << "Error de conexion" << std::endl;
				return MG_FALSE;
			}
			// Send request to the remote host.
			//std::cout << req_method.c_str() << " " << req_uri.c_str() << std:: endl << std::endl;
      mg_printf(server, "%s %s HTTP/1.0\r\nHost: %s\r\n\r\n",
                req_method.c_str(), req_uri.c_str(),
                s_remote_addr);
      return MG_TRUE;

    case MG_REPLY:
      // Send reply to the original connection
      orig = (struct mg_connection *) conn->connection_param;
			std::cout << "status code: " << conn->status_code << std::endl;
			std::cout << "content: " << conn->content << std::endl;
			std::cout << "fin reply" << std::endl;

			connection_active = false;
      return MG_TRUE;

    default:
      return MG_FALSE;
  }
}

int main(void) {
  s_server = mg_create_server(NULL, ev_handler);

  mg_set_option(s_server, "listening_port", "8080");
	
	std::string buffer;
	while(1){
		std::cout << "Ingresar request HTTP: ";
		getline(std::cin, buffer);
		//std::cout << "buffer: " << buffer << std::endl;
		if(!buffer.compare("exit")){
			break;
		}

		std::stringstream buffer_stream;
		buffer_stream << buffer;
		//std::cout << "buffer_stream: " << buffer_stream << std::endl;

		getline(buffer_stream, req_method, ' ');
		//std::cout << "req_method: " << req_method << std::endl;
		if(req_method.compare("GET") && req_method.compare("PUT") && req_method.compare("POST") && req_method.compare("DELETE")){
			std::cout << "Invalid method" << std::endl;
			continue;
		}
		
		getline(buffer_stream, req_uri);
		//std::cout << "req_uri: " << req_uri << std::endl;
		connection_active = connect();		

		while (connection_active) {
		  mg_poll_server(s_server, 1000);
		}
	}
  mg_destroy_server(&s_server);
  //printf("Existing on signal %d\n", s_received_signal);
	std::cout << "Exiting client" << std::endl;

  return EXIT_SUCCESS;
}
