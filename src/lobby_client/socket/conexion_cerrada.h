#ifndef CONEXION_CERRADA_H
#define CONEXION_CERRADA_H

#include <stdexcept>

class ClosedConnectionError : public std::exception {
   public:
    ClosedConnectionError() {}
    virtual const char* what() const noexcept { return "Cerraron la conexion"; }
    ~ClosedConnectionError() {}
};

#endif
