#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#define LOG_MODES 5

typedef enum Logmode{
    DEBUG,
    WARNING,
    ERROR,
    INFO,
    TRACE
} LOG_MODE;

class Logger
{
    public:

        virtual ~Logger();
        // tal vez agregar funcion 'clear' para borrar el singleton y liberar la memoria? en teoria es memoria no liberada, pero igual se liberaria solo cuando se cierra el juego asi q no cambia
        void setLogPath(std::string path) { log_path = path; }
        static Logger* getInstance();
        void Log(std::string comment, LOG_MODE mode);

    private:
        Logger();
        Logger* instancia;
        LOG_MODE mode;
        std::string log_path;
        std::string mode_symbols[LOG_MODES];
        std::string timestamp();
};

#endif // LOGGER_H

