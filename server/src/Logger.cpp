#include "Logger.h"
#include <fstream>
#include <ctime>

static Logger* instance = 0;

Logger::Logger()
{
    log_path = "log.txt";
    mode = WARNING; //cambiar para cambiar el default. en la version final el default deberia ser ERROR
    mode_symbols[0] = "D.\t";
    mode_symbols[1] = "W.\t";
    mode_symbols[2] = "E.\t";
    mode_symbols[3] = "I.\t";
    mode_symbols[4] = "T.\t";
    std::ofstream log_file(log_path.c_str(), std::ofstream::app);
    log_file << std::endl << "/---------------------- " << timestamp() << " ----------------------/" << std::endl;
    log_file.close();
}

Logger::~Logger()
{
    if(instancia != NULL)
        delete instancia;
    instancia = NULL;
}

std::string Logger::timestamp(){
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
    std::string str(buffer);

    return str;
}

void Logger::Log(std::string comment, LOG_MODE comment_mode){
    if(comment_mode >= mode){
        std::ofstream log_file(log_path.c_str(), std::ofstream::out | std::ofstream::app);
        log_file << mode_symbols[comment_mode] << comment << std::endl;
        log_file.close();
    }
}

Logger* Logger::getInstance(){
    if(!instance){
        instance = new Logger;
    }
    return instance;
}
