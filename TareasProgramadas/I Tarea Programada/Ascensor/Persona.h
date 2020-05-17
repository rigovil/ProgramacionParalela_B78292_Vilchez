#ifndef PERSONA_H
#define PERSONA_H

class Persona {

    public:
        Persona(int);
        virtual ~Persona();

        // ATRIBUTOS
        int id;
        int piso_actual;
        int piso_destino;
        int direccion;
        bool en_ascensor = false;
        bool fue_atendido = false;

        // METODOS
        void pulsarBoton();
        void subirse();
        void bajarse(int);
        void mensaje(int);  
        bool puedeSubir(int, int);
};

#endif 