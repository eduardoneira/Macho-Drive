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

//!Clase que maneja el logger
class Logger
{
    public:

        virtual ~Logger();
        // tal vez agregar funcion 'clear' para borrar el singleton y liberar la memoria? en teoria es memoria no liberada, pero igual se liberaria solo cuando se cierra el juego asi q no cambia
        //!Funcion que establece el logpath.
        void setLogPath(std::string path) { log_path = path; }
        //!Funcion que devuelve la instancia del logger.
        /*!Si no la encuentra la crea.
        */
        static Logger* getInstance();
        //!Funcion que genera el log.
        void Log(std::string comment, LOG_MODE mode);

    private:
        //!Funcion que inicializa el objeto.
        /*!Inicializa los simbolos de los modos, el logpath en "log.txt" y
        hace un log con el timestamp.
        */
        Logger();
        //!Variable con la instancia del logger.
        Logger* instancia;
        //!Variable con el modo del logger.
        LOG_MODE mode;
        //!Variabe con el logpath.
        std::string log_path;
        //!Variable con los simbolos de los modos.
        std::string mode_symbols[LOG_MODES];
        //!Funcion que genera un timestamp.
        std::string timestamp();
};

#endif // LOGGER_H

