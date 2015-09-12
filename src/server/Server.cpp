#include "Server.h"
#include <iostream>

Server::Server()
{
    server_mgr = new struct mg_mgr;
    mg_mgr_init(server_mgr, this);
    active = false;
}

Server::~Server()
{
    if(active){
        stop();
    }
}

void Server::poll(){
    mg_mgr_poll(server_mgr, 1000);
}

void Server::stop(){
    active = false;
    mg_mgr_free(server_mgr);
}

bool Server::createListeningConnection(const char * addr){
    //tal vez guardar la mg_connection como var de clase? todavia no se si hace falta
    struct mg_connection* nc = mg_bind(server_mgr, addr, Server::handlerCaller);
    if(nc == NULL){ //error
        return false;
    }
    mg_set_protocol_http_websocket(nc);
    //mg_enable_multithreading(nc);
    return true;
}

void Server::handlerCaller(struct mg_connection *nc, int ev, void* ev_data){
    Server* server_m = (Server*) nc->listener->mgr->user_data;
    //std::cout << "llego2" << std::endl;
    server_m->handler(nc, ev, ev_data);
}

void Server::handler(struct mg_connection* nc, int ev, void* ev_data){
    int connect_status;
    struct http_message* hmsg = (struct http_message*) ev_data;
    struct mg_serve_http_opts s;

    char content[100];
    int content_length = snprintf(content, sizeof(content), "Hello world");
    switch(ev){
        case NS_HTTP_REQUEST:
            //std::cout << "llego" << std::endl;
 
            if(mg_vcmp(&hmsg->uri, "/datos") == 0){
                mg_printf(nc, "HTTP/1.1 200 OK\r\n"
                                "Transfer-Encoding: chunked\r\n"
                                "\r\n");
                mg_printf_http_chunk(nc, "Hello world");
                mg_send_http_chunk(nc, "", 0);

            } else {
                mg_serve_http(nc, hmsg, s);
            }

            //usar mg_vcmp y los campos de http_message para ver que hacer
            //guardar mg_serve_https_opts y usar mg_serve_http para requests que no nos importan?
            break;
        default:
            break;
    }
}
