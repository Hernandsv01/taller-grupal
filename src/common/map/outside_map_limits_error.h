#ifndef CONEXION_CERRADA_H
#define CONEXION_CERRADA_H

#include <stdexcept>

class OutsideMapLimitsError : public std::exception {
   public:
    OutsideMapLimitsError() {}
    virtual const char* what() const noexcept {
        return "Se intentó acceder a un bloque fuera del limite del mapa";
    }
    ~OutsideMapLimitsError() {}
};

#endif
