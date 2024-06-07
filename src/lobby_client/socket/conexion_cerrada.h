#ifndef CONEXION_CERRADA_H
#define CONEXION_CERRADA_H

#include <stdexcept>


class ConexionCerradaError: public std::exception {
public:
    ConexionCerradaError() {}
    virtual const char* what() const noexcept { return "Cerraron la conexion"; }
    ~ConexionCerradaError() {}
};

#endif
