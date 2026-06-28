#ifndef PILA_H
#define PILA_H

template <typename T>
class Pila {
    struct NodoP {
        T dato;
        NodoP* sig;
        NodoP(T d, NodoP* s) : dato(d), sig(s) {}
    };
    NodoP* tope;

public:
    Pila() : tope(nullptr) {}

    
    ~Pila() {
        while (tope) {
            NodoP* t = tope;
            tope = tope->sig;
            delete t;
        }
    }

    void push(T d) { tope = new NodoP(d, tope); }

    void pop() {
        if (tope) {
            NodoP* t = tope;
            tope = tope->sig;
            delete t;
        }
    }

    
    T& top() {
        if (!tope) throw std::runtime_error("Pila vacia");
        return tope->dato;
    }

    bool vacia() const { return tope == nullptr; }
};
#endif
