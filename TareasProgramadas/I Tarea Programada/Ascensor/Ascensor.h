#ifndef ASCENSOR_H
#define ASCENSOR_H

#include "Persona.h"
#include <array>

#define SUBIR 1
#define MAX_CAPACIDAD 8

class Ascensor {

    public:
        Ascensor(int);
        virtual ~Ascensor();

        // ATRIBUTOS
        int capacidad;
        

        // METODOS
        bool lleno(); 
        bool vacio();
        bool terminar();
        bool puedenSubirPersonas();
        bool haySolicitudes();
        void operar();
        void subirPiso();
        void bajarPiso();
        void subirPersonas();
        void bajarPersonas();
        void mensaje(int);
        void borrarSolicitud(Persona *);
        void subirPersonaAlAscensor(Persona *);
        void decidirDireccion();
        int getPersonasAtendidas();

    private:
        int max_solicitudes;
        int personas_atendidas;
        int piso_actual;
        int direccion = SUBIR;
        std::array<Persona *, MAX_CAPACIDAD> personas_dentro;
};

#endif