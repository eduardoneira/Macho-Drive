#include "Client.h"
#include <iostream>

Client::Client()
{
    server_mgr = new struct mg_mgr;
    mg_mgr_init(server_mgr, this);
    active = false;
}

Client::~Client()
{
    if(active){
        stop();
    }
}

bool Client::connect(std::string addr){
    struct mg_connection *nc = mg_connect(server_mgr, addr.c_str(), Client::handlerCaller);
    if(!nc){
        return false;
    }
    mg_set_protocol_http_websocket(nc);
    //nc->user_data =
    //std::cout << "quiero conectar" << std::endl;
    return true;
}

void Client::handler(struct mg_connection* nc, int ev, void* ev_data){
    if(ev_data == NULL){
        std::cout << "ev_data NULL" << std::endl;
    }
    int connect_status = *(int*) ev_data;
    struct http_message* hmsg = (struct http_message*) ev_data;
    /*if(&(hmsg->message) != NULL){
        std::cout << "llega a handler con ";
        printf("%.*s", hmsg->message.len, hmsg->message.p);
    }*/
    std::cout << "fin" << std::endl;

    switch(ev){
        case NS_CONNECT:
            //connect_status = *(int*) ev_data;
            std::cout << "estoy conectado" << std::endl;
            if(connect_status == 0){
                std::cout << "enviando request" << std::endl;
                mg_printf(nc, "GET casas/ HTTP/1.1 200 OK\r\n\r\n\r\n");
                //mg_printf(nc, request.c_str());
            } else {
                std::cout << "error" << std::endl;
                //error
                break;
            }

            break;
        /*case NS_HTTP_REQUEST:
            hmsg = (struct http_message*) ev_data;
            //usar mg_vcmp y los campos de http_message para ver que hacer
            //guardar mg_serve_https_opts y usar mg_serve_http para requests que no nos importan?
            break;*/
        case NS_HTTP_REPLY:
            std::cout << "Got reply:" << std::endl;
            //std::cout << hmsg-> << std::endl << std::endl;
            printf("%.*s\n", hmsg->body.len, hmsg->body.p);
            //std::cout << hmsg->body.p << std::endl << std::endl;
            nc->flags |= NSF_SEND_AND_CLOSE;
            break;
        default:
            break;
    }
}

void Client::poll(){
    mg_mgr_poll(server_mgr, 1000);
}

void Client::stop(){
    active = false;
    mg_mgr_free(server_mgr);
}

bool Client::createListeningConnection(const char * addr){
    //tal vez guardar la mg_connection como var de clase? todavia no se si hace falta
    struct mg_connection* nc = mg_bind(server_mgr, addr, Client::handlerCaller);
    if(nc == NULL){ //error
        return false;
    }
    mg_set_protocol_http_websocket(nc);
    //mg_enable_multithreading(nc);
    return true;
}

void Client::handlerCaller(struct mg_connection *nc, int ev, void* ev_data){
    Client* server_m = (Client*) nc->mgr;
    if(server_m == NULL){
        std::cout << "server_m NULL" << std::endl;
    }
    //std::cout << "llego2" << std::endl;
    server_m->handler(nc, ev, ev_data);
}

