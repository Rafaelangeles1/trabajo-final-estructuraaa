#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <iostream>
#include <vector>
#include <string>
#include <functional>

template <typename K, typename V>
class HashTable {
private:
    struct NodoHash {
        K clave;
        V valor;
        bool ocupado;
        NodoHash() : ocupado(false) {}
    };

    std::vector<NodoHash> tabla;
    int capacidad;
    int tamanoActual;

    
    unsigned int funcionHashCompleja(const std::string& clave) const {
        unsigned int hashVal = 0;
        for (char ch : clave) {
            hashVal = 37 * hashVal + (unsigned char)ch;
        }
        
        hashVal ^= (hashVal >> 16);
        hashVal *= 0x45d9f3b;
        hashVal ^= (hashVal >> 16);
        return hashVal % (unsigned int)capacidad;
    }

    
    unsigned int funcionHashEntero(int clave) const {
        unsigned int k = (unsigned int)clave;
        k = ((k >> 16) ^ k) * 0x45d9f3b;
        k = ((k >> 16) ^ k) * 0x45d9f3b;
        k = (k >> 16) ^ k;
        return k % (unsigned int)capacidad;
    }

    
    unsigned int calcularHash(const std::string& clave) const {
        return funcionHashCompleja(clave);
    }
    unsigned int calcularHash(int clave) const {
        return funcionHashEntero(clave);
    }

public:
    
    HashTable(int cap = 101) : capacidad(cap), tamanoActual(0) {
        tabla.resize(capacidad);
    }

    
    void insertar(const K& clave, const V& valor) {
        unsigned int indice = calcularHash(clave);
        int intentos = 0;
        while (tabla[indice].ocupado && tabla[indice].clave != clave) {
            indice = (indice + 1) % capacidad; 
            intentos++;
            if (intentos >= capacidad) return; 
        }
        tabla[indice].clave = clave;
        tabla[indice].valor = valor;
        tabla[indice].ocupado = true;
        tamanoActual++;
    }

    
    V* buscar(const K& clave) {
        unsigned int indice = calcularHash(clave);
        int intentos = 0;
        while (tabla[indice].ocupado && intentos < capacidad) {
            if (tabla[indice].clave == clave) return &tabla[indice].valor;
            indice = (indice + 1) % capacidad;
            intentos++;
        }
        return nullptr;
    }

    
    void recorrer(std::function<void(const K&, const V&)> callback) const {
        for (int i = 0; i < capacidad; i++) {
            if (tabla[i].ocupado)
                callback(tabla[i].clave, tabla[i].valor);
        }
    }

    
    bool eliminar(const K& clave) {
        unsigned int indice = calcularHash(clave);
        int intentos = 0;
        while (tabla[indice].ocupado && intentos < capacidad) {
            if (tabla[indice].clave == clave) {
                tabla[indice].ocupado = false;
                tamanoActual--;
                return true;
            }
            indice = (indice + 1) % capacidad;
            intentos++;
        }
        return false;
    }

    // Operador asignacion
    HashTable& operator=(const HashTable& otro) {
        if (this != &otro) {
            tabla = otro.tabla;
            capacidad = otro.capacidad;
            tamanoActual = otro.tamanoActual;
        }
        return *this;
    }

    int size()    const { return tamanoActual; }
    bool vacia()  const { return tamanoActual == 0; }
};
#endif