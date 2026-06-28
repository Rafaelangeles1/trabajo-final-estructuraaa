#ifndef COLA_H
#define COLA_H

template <typename T>
class Cola {
    struct NodoC {
        T dato;
        NodoC* sig;
        NodoC(T d) : dato(d), sig(nullptr) {}
    };
    NodoC* frente;
    NodoC* final_;  

public:
    Cola() : frente(nullptr), final_(nullptr) {}

    
    ~Cola() {
        while (frente) {
            NodoC* t = frente;
            frente = frente->sig;
            delete t;
        }
    }

    void enqueue(T d) {
        NodoC* nuevo = new NodoC(d);
        if (!final_) frente = final_ = nuevo;
        else { final_->sig = nuevo; final_ = nuevo; }
    }

    void dequeue() {
        if (frente) {
            NodoC* t = frente;
            frente = frente->sig;
            if (!frente) final_ = nullptr;
            delete t;
        }
    }

   
    T& front() {
        if (!frente) throw std::runtime_error("Cola vacia");
        return frente->dato;
    }

    bool vacia() const { return frente == nullptr; }
};
#endif
