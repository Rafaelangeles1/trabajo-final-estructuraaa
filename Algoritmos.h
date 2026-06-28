#ifndef ALGORITMOS_H
#define ALGORITMOS_H
#include "ListaDoble.h"
#include "Entidades.h"
#include <iostream>
#include <vector>
#include <string>

// ============================================================
// RECURSIVIDAD: suma de puntos recorriendo nodos de la lista
// ============================================================
int sumaPuntosRec(Nodo<Usuario>* nodo) {
    if (!nodo) return 0;
    return nodo->dato.puntos + sumaPuntosRec(nodo->sig);
}

// ============================================================
// RECURSIVIDAD: contar elementos de un vector
// ============================================================
int contarPalabrasRec(const std::vector<std::string>& palabras, int idx = 0) {
    if (idx >= static_cast<int>(palabras.size())) return 0;
    return 1 + contarPalabrasRec(palabras, idx + 1);
}

// ============================================================
// RECURSIVIDAD: buscar item en vector
// ============================================================
bool buscarItemRec(const std::vector<std::string>& items,
    const std::string& buscado, int idx = 0) {
    if (idx >= static_cast<int>(items.size())) return false;
    if (items[idx] == buscado) return true;
    return buscarItemRec(items, buscado, idx + 1);
}

// ============================================================
// RECURSIVIDAD: buscar usuario por nombre en lista enlazada
// ============================================================
Nodo<Usuario>* buscarUsuarioRec(Nodo<Usuario>* nodo, const std::string& nombre) {
    if (!nodo) return nullptr;
    if (nodo->dato.nombre == nombre) return nodo;
    return buscarUsuarioRec(nodo->sig, nombre);
}

// ============================================================
// ORDENAMIENTO 1 - QuickSort (integrante Abel)
// Ordena vector<Ranking> por posicion - O(n log n) promedio
// ============================================================
void quickSort(std::vector<Ranking>& arr, int left, int right) {
    if (left >= right) return;
    int i = left, j = right;
    int pivot = arr[(left + right) / 2].posicion;
    while (i <= j) {
        while (arr[i].posicion < pivot) i++;
        while (arr[j].posicion > pivot) j--;
        if (i <= j) { std::swap(arr[i], arr[j]); i++; j--; }
    }
    if (left < j)  quickSort(arr, left, j);
    if (i < right) quickSort(arr, i, right);
}

// ============================================================
// ORDENAMIENTO 2 - ShellSort (integrante Abel)
// Ordena vector<Ranking> por posicion - O(n log^2 n)
// ============================================================
void shellSort(std::vector<Ranking>& arr) {
    int n = (int)arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            Ranking temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap].posicion > temp.posicion; j -= gap)
                arr[j] = arr[j - gap];
            arr[j] = temp;
        }
    }
}

// ============================================================
// ORDENAMIENTO 3 - MergeSort (integrante Martin Berrios Pacherres
//                 + Rafael David Prudencio Angeles Alegre)
// Funcion auxiliar merge - O(n)
// ============================================================
void merge(std::vector<Ranking>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    std::vector<Ranking> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].posicion <= R[j].posicion) { arr[k] = L[i]; i++; }
        else { arr[k] = R[j]; j++; }
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
}

// MergeSort recursivo - O(n log n) garantizado
// integrante Rafael David Prudencio Angeles Alegre
void mergeSort(std::vector<Ranking>& arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

// ============================================================
// LAMBDA HELPERS: funciones lambda reutilizables
// ============================================================

// Lambda: mostrar un usuario formateado
auto mostrarUsuario = [](const Usuario& u) {
    std::cout << "  - " << u.nombre
        << " | Puntos: " << u.puntos
        << " | Suscripcion: " << u.suscripcion << "\n";
    };

// Lambda: verificar si usuario es premium
auto esPremium = [](const Usuario& u) {
    return u.suscripcion != "Gratis";
    };

// Lambda: verificar si usuario supera umbral de puntos
auto superaUmbral = [](const Usuario& u, int umbral) {
    return u.puntos >= umbral;
    };

#endif // ALGORITMOS_H