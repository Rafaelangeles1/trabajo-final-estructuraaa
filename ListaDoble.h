#ifndef LISTADOBLE_H
#define LISTADOBLE_H
#include <functional>

template <typename T>
struct Nodo {
    T dato;
    Nodo* sig;
    Nodo* ant;
    Nodo(T d) : dato(d), sig(nullptr), ant(nullptr) {}
};

template <typename T>
class ListaDoble {
    Nodo<T>* inicio;
    Nodo<T>* fin;
    int tam;
public:
    ListaDoble() : inicio(nullptr), fin(nullptr), tam(0) {}

    
    ~ListaDoble() {
        Nodo<T>* aux = inicio;
        while (aux) {
            Nodo<T>* sig = aux->sig;
            delete aux;
            aux = sig;
        }
    }

    void agregar(T d) {
        Nodo<T>* nuevo = new Nodo<T>(d);
        if (!inicio) inicio = fin = nuevo;
        else { fin->sig = nuevo; nuevo->ant = fin; fin = nuevo; }
        tam++;
    }

    
    bool eliminar(std::function<bool(const T&)> predicado) {
        Nodo<T>* aux = inicio;
        while (aux) {
            if (predicado(aux->dato)) {
                if (aux->ant) aux->ant->sig = aux->sig;
                else inicio = aux->sig;
                if (aux->sig) aux->sig->ant = aux->ant;
                else fin = aux->ant;
                delete aux;
                tam--;
                return true;
            }
            aux = aux->sig;
        }
        return false;
    }

    
    Nodo<T>* buscar(std::function<bool(const T&)> predicado) {
        Nodo<T>* aux = inicio;
        while (aux) {
            if (predicado(aux->dato)) return aux;
            aux = aux->sig;
        }
        return nullptr;
    }

    void iterar(std::function<void(T&)> callback) {
        Nodo<T>* aux = inicio;
        while (aux) { callback(aux->dato); aux = aux->sig; }
    }

    
    void iterar(std::function<void(const T&)> callback) const {
        Nodo<T>* aux = inicio;
        while (aux) { callback(aux->dato); aux = aux->sig; }
    }

    void limpiar() {
        Nodo<T>* aux = inicio;
        while (aux) {
            Nodo<T>* sig = aux->sig;
            delete aux;
            aux = sig;
        }
        inicio = fin = nullptr;
        tam = 0;
    }

    Nodo<T>* getInicio() { return inicio; }
    int size() const { return tam; }
    bool vacia() const { return tam == 0; }
};
#endif
