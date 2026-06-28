// ============================================================
// main.cpp - Plataforma de Aprendizaje de Idiomas (Duolingo)
// Grupo 5 | Estructuras de Datos Avanzadas | UPC 2026-1
// Integrantes: Abel | Martin Berrios Pacherres |
//              Rafael David Prudencio Angeles Alegre
// ============================================================
#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <limits>
#include <algorithm> // Para std::reverse

#include "ListaDoble.h"
#include "Pila.h"
#include "Cola.h"
#include "Entidades.h"
#include "Algoritmos.h"
#include "GestionArchivos.h"
#include "HashTable.h"
#include "Generador.h"

using namespace std;

// ============================================================
// UTILIDADES DE CONSOLA
// ============================================================
void limpiar() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void linea(char c = '=', int n = 45) {
    cout << "  ";
    for (int i = 0; i < n; i++) cout << c;
    cout << "\n";
}

void titulo(const string& txt) {
    cout << "\n";
    linea();
    cout << "  " << txt << "\n";
    linea();
}

void pausar() {
    cout << "\n  Presiona ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ============================================================
// MENU PRINCIPAL
// ============================================================
void mostrarMenu() {
    cout << "\n";
    linea();
    cout << "       DUOLINGO - Aprendizaje de Idiomas\n";
    linea();
    cout << "  [1] Agregar usuario\n";
    cout << "  [2] Ver usuarios y total de puntos\n";
    cout << "  [3] Buscar usuario\n";
    cout << "  [4] Eliminar usuario\n";
    cout << "  [5] Simular leccion (Vocabulario)\n";
    cout << "  [6] Ver notificaciones\n";
    cout << "  [7] Ver ranking (Elegir Algoritmo)\n";
    cout << "  [8] Buscar item en tienda\n";
    cout << "  [9] Gestionar idiomas\n";
    cout << " [10] Generar dataset\n";
    cout << " [11] Estadisticas del sistema\n";
    cout << "  [0] Guardar y salir\n";
    linea();
    cout << "  Opcion: ";
}

// ============================================================
// MAIN
// ============================================================
int main() {
    // --- Cargar usuarios desde archivo ---
    ListaDoble<Usuario> listaUsuarios;
    cargarUsuarios(listaUsuarios);

    // --- Estructuras de datos del sistema ---
    Pila<Vocabulario>    pilaVocab;
    Cola<Notificacion>   colaNotif;
    ListaDoble<Idioma>   listaIdiomas;
    ListaDoble<Logro>    listaLogros;

    // --- HashTable: busqueda O(1) de usuarios por nombre ---
    HashTable<string, Usuario> hashUsuarios;

    // --- Cargar HashTable con los usuarios del archivo ---
    listaUsuarios.iterar([&hashUsuarios](Usuario& u) {
        hashUsuarios.insertar(u.nombre, u);
        });

    // --- Generador de dataset ---
    Generador gen(42);

    // --- Cargar idiomas base ---
    listaIdiomas.agregar({ "Ingles",    "Facil" });
    listaIdiomas.agregar({ "Frances",   "Medio" });
    listaIdiomas.agregar({ "Japones",   "Dificil" });
    listaIdiomas.agregar({ "Aleman",    "Medio" });
    listaIdiomas.agregar({ "Portugues", "Facil" });

    // --- Cargar logros base ---
    listaLogros.agregar({ "Primera leccion", "Completa tu primera leccion" });
    listaLogros.agregar({ "Racha 7 dias",    "7 dias consecutivos" });
    listaLogros.agregar({ "100 puntos",      "Acumula 100 puntos" });

    int op = -1;

    do {
        mostrarMenu();
        cin >> op;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n  Entrada invalida. Intente de nuevo.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // --------------------------------------------------------
        switch (op) {

            // ========================================================
            // OPCION 1: Agregar usuario
            // ========================================================
        case 1: {
            titulo("AGREGAR USUARIO");
            string nom, sus;
            int pts;

            cout << "  Nombre: "; getline(cin, nom);
            if (nom.empty()) { cout << "  Nombre no puede estar vacio.\n"; break; }

            cout << "  Puntos iniciales: "; cin >> pts;
            if (cin.fail() || pts < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Puntos invalidos. Se asigna 0.\n";
                pts = 0;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "  Suscripcion (Gratis/Plus/Premium): "; getline(cin, sus);
            if (sus.empty()) sus = "Gratis";

            // Verificar duplicado en HashTable - O(1)
            Usuario* existente = hashUsuarios.buscar(nom);
            if (existente) {
                cout << "  El usuario \"" << nom << "\" ya existe.\n";
                break;
            }

            Usuario nuevo = { nom, pts, sus };
            listaUsuarios.agregar(nuevo);
            hashUsuarios.insertar(nom, nuevo);

            // LAMBDA: confirmar registro
            auto confirmar = [](const string& nombre, const string& sus_) {
                cout << "  Usuario \"" << nombre
                    << "\" registrado con suscripcion " << sus_ << ".\n";
                };
            confirmar(nom, sus);
            break;
        }

              // ========================================================
              // OPCION 2: Ver usuarios y total de puntos
              // ========================================================
        case 2: {
            titulo("USUARIOS REGISTRADOS");
            if (listaUsuarios.vacia()) {
                cout << "  No hay usuarios registrados.\n"; break;
            }

            int conta = 0;
            // LAMBDA: mostrar usuario
            listaUsuarios.iterar([&conta](Usuario& u) {
                conta++;
                cout << "  " << setw(2) << conta << ". "
                    << left << setw(16) << u.nombre
                    << " | Pts: " << setw(5) << u.puntos
                    << " | " << u.suscripcion << "\n";
                });

            int total = sumaPuntosRec(listaUsuarios.getInicio());
            linea('-');
            cout << "  Total de usuarios : " << listaUsuarios.size() << "\n";
            cout << "  Total puntos      : " << total << "\n";

            // LAMBDA: contar premium
            int premiums = 0;
            listaUsuarios.iterar([&premiums](Usuario& u) {
                if (esPremium(u)) premiums++;
                });
            cout << "  Usuarios premium  : " << premiums << "\n";
            break;
        }

              // ========================================================
              // OPCION 3: Buscar usuario (HashTable O(1) + recursion)
              // ========================================================
        case 3: {
            titulo("BUSCAR USUARIO");
            string nom;
            cout << "  Nombre a buscar: "; getline(cin, nom);

            // Busqueda O(1) con HashTable
            Usuario* ptr = hashUsuarios.buscar(nom);
            if (ptr) {
                cout << "\n  [HashTable O(1)] Usuario encontrado:\n";
                cout << "    Nombre      : " << ptr->nombre << "\n";
                cout << "    Puntos      : " << ptr->puntos << "\n";
                cout << "    Suscripcion : " << ptr->suscripcion << "\n";
            }
            else {
                // Busqueda recursiva como respaldo
                Nodo<Usuario>* nRec = buscarUsuarioRec(listaUsuarios.getInicio(), nom);
                if (nRec)
                    cout << "  [Busqueda recursiva] Encontrado: "
                    << nRec->dato.nombre << " | " << nRec->dato.puntos << " pts\n";
                else
                    cout << "  Usuario \"" << nom << "\" no encontrado.\n";
            }
            break;
        }

              // ========================================================
              // OPCION 4: Eliminar usuario
              // ========================================================
        case 4: {
            titulo("ELIMINAR USUARIO");
            string nom;
            cout << "  Nombre a eliminar: "; getline(cin, nom);

            // LAMBDA predicado para eliminar
            bool ok = listaUsuarios.eliminar([&nom](const Usuario& u) {
                return u.nombre == nom;
                });
            if (ok) {
                hashUsuarios.recorrer([](const string&, const Usuario&) {});
                // Reconstruir hashtable (simple para este proyecto)
                HashTable<string, Usuario> nuevo;
                listaUsuarios.iterar([&nuevo](Usuario& u) {
                    nuevo.insertar(u.nombre, u);
                    });
                hashUsuarios = nuevo;
                cout << "  Usuario \"" << nom << "\" eliminado correctamente.\n";
            }
            else {
                cout << "  Usuario \"" << nom << "\" no encontrado.\n";
            }
            break;
        }

              // ========================================================
              // OPCION 5: Simular leccion con vocabulario (Pila)
              // ========================================================
        case 5: {
            titulo("SIMULACION DE LECCION");

            // Generar vocabulario con el generador
            auto vocab = gen.generarVocabulario(6);
            for (auto& v : vocab) pilaVocab.push(v);

            // Agregar algunas palabras fijas tambien
            pilaVocab.push({ "Hello",   "Hola" });
            pilaVocab.push({ "Apple",   "Manzana" });
            pilaVocab.push({ "Dog",     "Perro" });
            pilaVocab.push({ "School",  "Escuela" });
            pilaVocab.push({ "Water",   "Agua" });

            cout << "  Estudia estas palabras:\n\n";
            int num = 1;
            while (!pilaVocab.vacia()) {
                cout << "  " << num++ << ". "
                    << left << setw(12) << pilaVocab.top().palabra
                    << " => " << pilaVocab.top().traduccion << "\n";
                pilaVocab.pop();
            }

            // Miniexamen con lambda validador
            cout << "\n  Mini examen: escribe la traduccion de 'Dog'\n";
            cout << "  Tu respuesta: ";
            string resp; getline(cin, resp);

            auto validar = [](const string& r) {
                return r == "Perro" || r == "perro" || r == "PERRO";
                };
            cout << (validar(resp) ? "  Correcto! +10 puntos\n" : "  Incorrecto. Era: Perro\n");
            break;
        }

              // ========================================================
              // OPCION 6: Notificaciones (Cola)
              // ========================================================
        case 6: {
            titulo("NOTIFICACIONES");

            // Generar notificaciones con el generador
            auto notifs = gen.generarNotificaciones(4);
            for (auto& n : notifs)
                colaNotif.enqueue({ n.mensaje, n.hora });

            // Agregar notificaciones del sistema
            colaNotif.enqueue({ "Nueva racha disponible!",      "10:00" });
            colaNotif.enqueue({ "Completa tu leccion diaria.",  "12:00" });
            colaNotif.enqueue({ "Tu amigo te supero en puntos!","15:30" });

            cout << "  Procesando notificaciones pendientes:\n\n";
            int i = 1;
            while (!colaNotif.vacia()) {
                cout << "  " << i++ << ". ["
                    << colaNotif.front().hora << "] "
                    << colaNotif.front().mensaje << "\n";
                colaNotif.dequeue();
            }
            cout << "\n  Todas las notificaciones procesadas.\n";
            break;
        }

              // ========================================================
              // OPCION 7: Ranking con selección de Algoritmo interactivo
              // ========================================================
        case 7: {
            titulo("RANKING DE USUARIOS");

            // Generar ranking con el generador
            auto rankGen = gen.generarRanking(5);
            vector<Ranking> rank;
            for (auto& r : rankGen)
                rank.push_back({ r.usuario, r.posicion });

            // Agregar usuarios registrados al ranking
            listaUsuarios.iterar([&rank](Usuario& u) {
                rank.push_back({ u.nombre, u.puntos });
                });

            if (rank.empty()) { cout << "  Sin datos de ranking.\n"; break; }

            // Menú de selección de algoritmos
            cout << "  Seleccione el algoritmo de ordenamiento:\n";
            cout << "  [1] QuickSort (Predeterminado - In-place)\n";
            cout << "  [2] MergeSort (Estable - O(n log n) garantizado)\n";
            cout << "  [3] ShellSort (Sondeo por brechas - Eficiente)\n";
            cout << "  Selección: ";
            int algOp; cin >> algOp;
            if (cin.fail()) {
                cin.clear();
                algOp = 1;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (algOp) {
            case 2:
                cout << "\n  Ordenando de forma estable con MergeSort...\n";
                mergeSort(rank, 0, (int)rank.size() - 1);
                break;
            case 3:
                cout << "\n  Ordenando eficientemente con ShellSort...\n";
                shellSort(rank);
                break;
            case 1:
            default:
                cout << "\n  Ordenando velozmente con QuickSort...\n";
                quickSort(rank, 0, (int)rank.size() - 1);
                break;
            }

            // Invertir para mostrar el puntaje mayor primero
            reverse(rank.begin(), rank.end());

            cout << "\n";
            for (int i = 0; i < (int)rank.size() && i < 10; i++) {
                cout << "  #" << setw(2) << (i + 1) << " "
                    << left << setw(16) << rank[i].usuario
                    << " | " << rank[i].posicion << " pts\n";
            }
            break;
        }

              // ========================================================
              // OPCION 8: Buscar item en tienda (recursion)
              // ========================================================
        case 8: {
            titulo("TIENDA DE ITEMS");

            // Generar items con el generador
            auto itemsGen = gen.generarItems();
            vector<string> items;
            cout << "  Items disponibles:\n";
            for (int i = 0; i < (int)itemsGen.size(); i++) {
                items.push_back(itemsGen[i].tipo);
                cout << "  " << (i + 1) << ". "
                    << left << setw(20) << itemsGen[i].tipo
                    << " | " << "---" << " gemas\n";
            }

            cout << "\n  Buscar item: ";
            string buscar; getline(cin, buscar);

            bool encontrado = buscarItemRec(items, buscar);
            cout << (encontrado
                ? "  Item \"" + buscar + "\" encontrado en la tienda.\n"
                : "  Item \"" + buscar + "\" NO disponible.\n");
            cout << "  Total de items: " << contarPalabrasRec(items) << "\n";
            break;
        }

              // ========================================================
              // OPCION 9: Gestionar idiomas (ListaDoble)
              // ========================================================
        case 9: {
            titulo("IDIOMAS DISPONIBLES");
            cout << "  Idiomas en el sistema:\n\n";
            int i = 1;
            listaIdiomas.iterar([&i](const Idioma& id) {
                cout << "  " << i++ << ". "
                    << left << setw(12) << id.nombre
                    << " | Dificultad: " << id.dificultad << "\n";
                });

            // LAMBDA: filtrar solo idiomas dificiles
            cout << "\n  Idiomas de dificultad 'Dificil':\n";
            listaIdiomas.iterar([](const Idioma& id) {
                if (id.dificultad == "Dificil")
                    cout << "  -> " << id.nombre << "\n";
                });

            // Buscar idioma
            cout << "\n  Buscar idioma: ";
            string nom; getline(cin, nom);
            Nodo<Idioma>* res = listaIdiomas.buscar([&nom](const Idioma& id) {
                return id.nombre == nom;
                });
            if (res)
                cout << "  Encontrado: " << res->dato.nombre
                << " (" << res->dato.dificultad << ")\n";
            else
                cout << "  Idioma no encontrado.\n";
            break;
        }

              // ========================================================
              // OPCION 10: Generar dataset completo
              // ========================================================
        case 10: {
            titulo("GENERADOR DE DATASET");
            cout << "  Cuantos usuarios generar? (5-50): ";
            int n; cin >> n;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (n < 5 || n > 50) { cout << "  Valor fuera de rango. Se usa 10.\n"; n = 10; }

            auto usuariosGen = gen.generarUsuarios(n);
            cout << "\n  Usuarios generados:\n";
            for (int i = 0; i < (int)usuariosGen.size() && i < 8; i++) {
                cout << "  " << (i + 1) << ". "
                    << left << setw(20) << usuariosGen[i].nombre
                    << " | " << setw(4) << usuariosGen[i].puntos << " pts"
                    << " | " << usuariosGen[i].suscripcion << "\n";
            }
            if ((int)usuariosGen.size() > 8)
                cout << "  ... y " << (usuariosGen.size() - 8) << " mas.\n";

            cout << "\n  Cargar estos usuarios al sistema? (s/n): ";
            char resp; cin >> resp;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (resp == 's' || resp == 'S') {
                for (auto& u : usuariosGen) {
                    // Solo agregar si no existe
                    if (!hashUsuarios.buscar(u.nombre)) {
                        listaUsuarios.agregar(u);
                        hashUsuarios.insertar(u.nombre, u);
                    }
                }
                cout << "  Usuarios cargados al sistema.\n";
            }

            // Guardar archivos de dataset
            gen.guardarUsuariosEnArchivo(usuariosGen, "dataset_usuarios.txt");
            gen.guardarLeccionesEnArchivo(gen.generarLecciones(20), "dataset_lecciones.txt");
            gen.guardarVocabularioEnArchivo(gen.generarVocabulario(30), "dataset_vocabulario.txt");
            break;
        }

               // ========================================================
               // OPCION 11: Estadisticas del sistema
               // ========================================================
        case 11: {
            titulo("ESTADISTICAS DEL SISTEMA");

            int totalUsuarios = listaUsuarios.size();
            int totalPuntos = sumaPuntosRec(listaUsuarios.getInicio());
            int premiums = 0;
            int maxPuntos = 0;
            string mejorUsuario = "N/A";

            // LAMBDA: calcular estadisticas en una sola pasada
            listaUsuarios.iterar([&](Usuario& u) {
                if (esPremium(u)) premiums++;
                if (u.puntos > maxPuntos) {
                    maxPuntos = u.puntos;
                    mejorUsuario = u.nombre;
                }
                });

            linea('-');
            cout << "  Total usuarios    : " << totalUsuarios << "\n";
            cout << "  Usuarios premium  : " << premiums << "\n";
            cout << "  Usuarios gratis   : " << (totalUsuarios - premiums) << "\n";
            cout << "  Total puntos      : " << totalPuntos << "\n";
            cout << "  Promedio puntos   : "
                << (totalUsuarios > 0 ? totalPuntos / totalUsuarios : 0) << "\n";
            cout << "  Lider de puntos   : " << mejorUsuario
                << " (" << maxPuntos << " pts)\n";
            linea('-');
            cout << "  Idiomas cargados  : " << listaIdiomas.size() << "\n";
            cout << "  Logros del sistema: " << listaLogros.size() << "\n";
            linea('-');

            // Mostrar logros
            cout << "  Logros disponibles:\n";
            listaLogros.iterar([](const Logro& l) {
                cout << "    * " << l.nombre << ": " << l.descripcion << "\n";
                });
            break;
        }

               // ========================================================
               // OPCION 0: Guardar y salir
               // ========================================================
        case 0:
            guardarUsuarios(listaUsuarios);
            cout << "\n  Datos guardados correctamente.\n";
            cout << "  Hasta luego!\n\n";
            break;

        default:
            cout << "\n  Opcion invalida. Elige entre 0 y 11.\n";
            break;
        }

        if (op != 0) pausar();

    } while (op != 0);

    return 0;
}