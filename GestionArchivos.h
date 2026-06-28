#ifndef GESTIONARCHIVOS_H
#define GESTIONARCHIVOS_H
#include <fstream>
#include <iostream>
#include "ListaDoble.h"
#include "Entidades.h"

void guardarUsuarios(ListaDoble<Usuario>& lista) {
    std::ofstream archivo("usuarios.txt");
    if (!archivo.is_open()) {
        std::cerr << "Error: no se pudo abrir usuarios.txt para escritura.\n";
        return;
    }
    lista.iterar([&archivo](Usuario& u) {
        archivo << u.nombre << " " << u.puntos << " " << u.suscripcion << "\n";
        });
    archivo.close();
}

void cargarUsuarios(ListaDoble<Usuario>& lista) {
    std::ifstream archivo("usuarios.txt");
    if (!archivo.is_open()) return;

    
    lista.limpiar();

    std::string n, s;
    int p;
    while (archivo >> n >> p >> s) {
        lista.agregar({ n, p, s });
    }
    archivo.close();
}

#endif
