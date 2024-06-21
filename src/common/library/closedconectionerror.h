#ifndef CLOSED_CONECTION_ERROR_H
#define CLOSED_CONECTION_ERROR_H

#include <stdexcept>

class ClosedConnectionError : public std::exception {
   public:
    ClosedConnectionError() {}
    virtual const char* what() const noexcept { return "Cerraron la conexion"; }
    ~ClosedConnectionError() {}
};

#endif
