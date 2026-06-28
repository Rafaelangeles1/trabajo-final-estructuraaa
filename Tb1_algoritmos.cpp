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
    cout << "  [7] Ver ranking\n";
    cout << "  [8] Ver progreso de usuario\n";
    cout << "  [9] Gestionar idiomas\n";
    cout << " [10] Estadisticas del sistema\n";
    cout << " [11] Generar dataset\n";
    cout << "  [0] Guardar y salir\n";
    linea();
    cout << "  Opcion: ";
}


int main() {
    
    Generador gen(42);
    gen.generarTodo(20, 20, 30);   // genera y guarda archivos

    
    ListaDoble<Usuario>    listaUsuarios;
    ListaDoble<Idioma>     listaIdiomas;
    ListaDoble<Logro>      listaLogros;
    Pila<Vocabulario>      pilaVocab;
    Cola<Notificacion>     colaNotif;
    HashTable<string, Usuario> hashUsuarios;

    // Cargar usuarios desde archivo (los que genero + los previos)
    cargarUsuarios(listaUsuarios);

    // Sincronizar HashTable con la lista cargada
    listaUsuarios.iterar([&hashUsuarios](Usuario& u) {
        hashUsuarios.insertar(u.nombre, u);
        });

    // Idiomas del sistema
    auto idiomasGen = gen.generarIdiomas();
    for (auto& id : idiomasGen) listaIdiomas.agregar(id);

    // Logros del sistema
    auto logrosGen = gen.generarLogros();
    for (auto& l : logrosGen) listaLogros.agregar(l);

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

        switch (op) {

            
        case 1: {
            titulo("AGREGAR USUARIO");
            string nom, sus;
            int pts;

            cout << "  Nombre: ";           getline(cin, nom);
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

            
            if (hashUsuarios.buscar(nom)) {
                cout << "  El usuario \"" << nom << "\" ya existe.\n";
                break;
            }

            Usuario nuevo = { nom, pts, sus };
            listaUsuarios.agregar(nuevo);
            hashUsuarios.insertar(nom, nuevo);

            
            auto confirmar = [](const string& n, const string& s) {
                cout << "  Usuario \"" << n
                    << "\" registrado con suscripcion " << s << ".\n";
                };
            confirmar(nom, sus);
            break;
        }

     
        case 2: {
            titulo("USUARIOS REGISTRADOS");
            if (listaUsuarios.vacia()) {
                cout << "  No hay usuarios registrados.\n"; break;
            }

            int conta = 0;
            
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

            //contar usuarios premium
            int premiums = 0;
            listaUsuarios.iterar([&premiums](Usuario& u) {
                if (esPremium(u)) premiums++;
                });
            cout << "  Usuarios premium  : " << premiums << "\n";
            break;
        }

             
        case 3: {
            titulo("BUSCAR USUARIO");
            string nom;
            cout << "  Nombre a buscar: "; getline(cin, nom);

            // Busqueda con HashTable
            Usuario* ptr = hashUsuarios.buscar(nom);
            if (ptr) {
                cout << "\n  [HashTable O(1)] Usuario encontrado:\n";
                cout << "    Nombre      : " << ptr->nombre << "\n";
                cout << "    Puntos      : " << ptr->puntos << "\n";
                cout << "    Suscripcion : " << ptr->suscripcion << "\n";
            }
            else {
                // Recursion: busqueda por lista enlazada
                Nodo<Usuario>* nRec = buscarUsuarioRec(listaUsuarios.getInicio(), nom);
                if (nRec)
                    cout << "  [Busqueda recursiva] Encontrado: "
                    << nRec->dato.nombre << " | "
                    << nRec->dato.puntos << " pts\n";
                else
                    cout << "  Usuario \"" << nom << "\" no encontrado.\n";
            }
            break;
        }

              
        case 4: {
            titulo("ELIMINAR USUARIO");
            string nom;
            cout << "  Nombre a eliminar: "; getline(cin, nom);

            
            bool ok = listaUsuarios.eliminar([&nom](const Usuario& u) {
                return u.nombre == nom;
                });
            if (ok) {
                
                HashTable<string, Usuario> nueva;
                listaUsuarios.iterar([&nueva](Usuario& u) {
                    nueva.insertar(u.nombre, u);
                    });
                hashUsuarios = nueva;
                cout << "  Usuario \"" << nom << "\" eliminado correctamente.\n";
            }
            else {
                cout << "  Usuario \"" << nom << "\" no encontrado.\n";
            }
            break;
        }

              
        case 5: {
            titulo("SIMULACION DE LECCION");

            // Vocabulario generado + palabras fijas
            auto vocab = gen.generarVocabulario(5);
            for (auto& v : vocab)          pilaVocab.push(v);
            pilaVocab.push({ "Hello",  "Hola" });
            pilaVocab.push({ "Apple",  "Manzana" });
            pilaVocab.push({ "Dog",    "Perro" });
            pilaVocab.push({ "School", "Escuela" });
            pilaVocab.push({ "Water",  "Agua" });

            cout << "  Estudia estas palabras:\n\n";
            int num = 1;
            while (!pilaVocab.vacia()) {
                cout << "  " << num++ << ". "
                    << left << setw(12) << pilaVocab.top().palabra
                    << " => " << pilaVocab.top().traduccion << "\n";
                pilaVocab.pop();
            }

            //validar respuesta del mini-examen
            cout << "\n  Mini examen: escribe la traduccion de 'Dog'\n";
            cout << "  Tu respuesta: ";
            string resp; getline(cin, resp);
            auto validar = [](const string& r) {
                return r == "Perro" || r == "perro" || r == "PERRO";
                };
            cout << (validar(resp)
                ? "  Correcto! +10 puntos\n"
                : "  Incorrecto. Era: Perro\n");
            break;
        }

              
        case 6: {
            titulo("NOTIFICACIONES");

            // Cargar notificaciones generadas + del sistema
            auto notifs = gen.generarNotificaciones(4);
            for (auto& n : notifs)
                colaNotif.enqueue({ n.mensaje, n.hora });
            colaNotif.enqueue({ "Nueva racha disponible!",       "10:00" });
            colaNotif.enqueue({ "Completa tu leccion diaria.",   "12:00" });
            colaNotif.enqueue({ "Tu amigo te supero en puntos!", "15:30" });

            cout << "  Procesando notificaciones (FIFO):\n\n";
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

              
        case 7: {
            titulo("RANKING DE USUARIOS");

            // Construir vector de ranking
            vector<Ranking> rank;
            listaUsuarios.iterar([&rank](Usuario& u) {
                rank.push_back({ u.nombre, u.puntos });
                });
            // Agregar algunos del generador
            auto rankGen = gen.generarRanking(5);
            for (auto& r : rankGen) rank.push_back({ r.usuario, r.posicion });

            if (rank.empty()) { cout << "  Sin datos de ranking.\n"; break; }

            // Submenu de algoritmo
            cout << "\n  Ordenar con:\n";
            cout << "  QuickSort promedio\n";
            cout << "  ShellSort\n";
            cout << "  MergeSort garantizado\n";
            cout << "  Algoritmo: ";
            int alg; cin >> alg;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (alg == 1) { quickSort(rank, 0, (int)rank.size() - 1); cout << "  QuickSort aplicado\n"; }
            else if (alg == 2) { shellSort(rank);                         cout << "  ShellSort aplicado\n"; }
            else if (alg == 3) { mergeSort(rank, 0, (int)rank.size() - 1); cout << "  MergeSort aplicado\n"; }
            else { quickSort(rank, 0, (int)rank.size() - 1); cout << "  QuickSort por defecto\n"; }

            // Mostrar de mayor a menor
            reverse(rank.begin(), rank.end());
            cout << "\n";
            for (int i = 0; i < (int)rank.size() && i < 10; i++) {
                cout << "  #" << setw(2) << (i + 1) << " "
                    << left << setw(16) << rank[i].usuario
                    << " | " << rank[i].posicion << " pts\n";
            }
            break;
        }

              
        case 8: {
            titulo("PROGRESO DE USUARIO");

            if (listaUsuarios.vacia()) {
                cout << "  No hay usuarios registrados.\n"; break;
            }

            cout << "  Nombre del usuario: ";
            string nom; getline(cin, nom);

            //buscar usuario por nombre en la lista
            Nodo<Usuario>* nodo = listaUsuarios.buscar([&nom](const Usuario& u) {
                return u.nombre == nom;
                });

            if (!nodo) {
                cout << "  Usuario \"" << nom << "\" no encontrado.\n"; break;
            }

            // Generar progreso del usuario con el generador
            auto progresosGen = gen.generarProgreso(1);
            Progreso prog = progresosGen[0];
            prog.fecha = "2026-06-28";   // fecha actual del sistema

            
            int totalLecciones = gen.generarLecciones(20).size();
            auto calcPorcentaje = [](int completadas, int total, auto& self) -> int {
                if (total <= 0) return 0;
                if (completadas <= 0) return 0;
                if (completadas >= total) return 100;
                // Suma recursiva de porcentaje por leccion
                return 1 + self(completadas - 1, total, self);
                };
            int porcentaje = calcPorcentaje(
                prog.lecciones_completadas, totalLecciones, calcPorcentaje);
            // Escalar al rango 0-100
            porcentaje = (prog.lecciones_completadas * 100) / max(1, totalLecciones);

            // Mostrar progreso
            linea('-');
            cout << "  Usuario          : " << nodo->dato.nombre << "\n";
            cout << "  Suscripcion      : " << nodo->dato.suscripcion << "\n";
            cout << "  Puntos totales   : " << nodo->dato.puntos << "\n";
            linea('-');
            cout << "  Ultima actividad : " << prog.fecha << "\n";
            cout << "  Lecciones hechas : " << prog.lecciones_completadas
                << " / " << totalLecciones << "\n";
            cout << "  Avance           : " << porcentaje << "%\n";
            linea('-');

            // Barra de progreso visual
            cout << "  [";
            int llenos = porcentaje / 5;   
            for (int i = 0; i < 20; i++)
                cout << (i < llenos ? "#" : "-");
            cout << "] " << porcentaje << "%\n";
            linea('-');

            //mensaje segun nivel de avance
            auto mensajeNivel = [](int pct) -> string {
                if (pct >= 80) return "Excelente progreso! Sigue asi.";
                if (pct >= 50) return "Buen avance, no te detengas!";
                if (pct >= 20) return "Vas por buen camino, practica mas.";
                return "Recien empezando, cada leccion cuenta!";
                };
            cout << "  " << mensajeNivel(porcentaje) << "\n";
            break;
        }

              
        case 9: {
            titulo("IDIOMAS DISPONIBLES");
            cout << "  Idiomas en el sistema:\n\n";
            int i = 1;
            listaIdiomas.iterar([&i](const Idioma& id) {
                cout << "  " << i++ << ". "
                    << left << setw(12) << id.nombre
                    << " | Dificultad: " << id.dificultad << "\n";
                });

            
            cout << "\n  Idiomas nivel Dificil:\n";
            listaIdiomas.iterar([](const Idioma& id) {
                if (id.dificultad == "Dificil")
                    cout << "  -> " << id.nombre << "\n";
                });

            
            cout << "\n  Buscar idioma (o ENTER para omitir): ";
            string nom; getline(cin, nom);
            if (!nom.empty()) {
                Nodo<Idioma>* res = listaIdiomas.buscar([&nom](const Idioma& id) {
                    return id.nombre == nom;
                    });
                if (res)
                    cout << "  Encontrado: " << res->dato.nombre
                    << " (" << res->dato.dificultad << ")\n";
                else
                    cout << "  Idioma \"" << nom << "\" no encontrado.\n";
            }
            break;
        }

              
        case 10: {
            titulo("ESTADISTICAS DEL SISTEMA");

            int totalUsuarios = listaUsuarios.size();
            int totalPuntos = sumaPuntosRec(listaUsuarios.getInicio());
            int premiums = 0;
            int maxPuntos = 0;
            string lider = "N/A";

            
            listaUsuarios.iterar([&](Usuario& u) {
                if (esPremium(u)) premiums++;
                if (u.puntos > maxPuntos) { maxPuntos = u.puntos; lider = u.nombre; }
                });

            linea('-');
            cout << "  Total usuarios    : " << totalUsuarios << "\n";
            cout << "  Usuarios premium  : " << premiums << "\n";
            cout << "  Usuarios gratis   : " << (totalUsuarios - premiums) << "\n";
            cout << "  Total puntos      : " << totalPuntos << "\n";
            cout << "  Promedio puntos   : "
                << (totalUsuarios > 0 ? totalPuntos / totalUsuarios : 0) << "\n";
            cout << "  Lider de puntos   : " << lider
                << " (" << maxPuntos << " pts)\n";
            linea('-');
            cout << "  Idiomas cargados  : " << listaIdiomas.size() << "\n";
            cout << "  Logros del sistema: " << listaLogros.size() << "\n";
            linea('-');
            cout << "  Logros disponibles:\n";
            listaLogros.iterar([](const Logro& l) {
                cout << "    * " << l.nombre << ": " << l.descripcion << "\n";
                });
            break;
        }

               
        case 11: {
            titulo("GENERADOR DE DATASET");
            cout << "  Cuantos usuarios generar? (5-50): ";
            int n; cin >> n;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (n < 5 || n > 50) { cout << "  Fuera de rango. Se usa 10.\n"; n = 10; }

            auto usuariosGen = gen.generarUsuarios(n);
            auto leccionesGen = gen.generarLecciones(20);
            auto vocabGen = gen.generarVocabulario(30);

            // Mostrar preview de usuarios generados
            cout << "\n  Usuarios generados:\n";
            for (int i = 0; i < (int)usuariosGen.size() && i < 8; i++) {
                cout << "  " << (i + 1) << ". "
                    << left << setw(20) << usuariosGen[i].nombre
                    << " | " << setw(4) << usuariosGen[i].puntos << " pts"
                    << " | " << usuariosGen[i].suscripcion << "\n";
            }
            if ((int)usuariosGen.size() > 8)
                cout << "  ... y " << (usuariosGen.size() - 8) << " mas.\n";

            // Guardar los tres archivos
            gen.guardarUsuariosEnArchivo(usuariosGen, "dataset_usuarios.txt");
            gen.guardarLeccionesEnArchivo(leccionesGen, "dataset_lecciones.txt");
            gen.guardarVocabularioEnArchivo(vocabGen, "dataset_vocabulario.txt");

            // Preguntar si cargar al sistema
            cout << "\n  Cargar usuarios al sistema? (s/n): ";
            char resp; cin >> resp;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (resp == 's' || resp == 'S') {
                int cargados = 0;
                for (auto& u : usuariosGen) {
                    if (!hashUsuarios.buscar(u.nombre)) {
                        listaUsuarios.agregar(u);
                        hashUsuarios.insertar(u.nombre, u);
                        cargados++;
                    }
                }
                cout << "  " << cargados << " usuarios nuevos cargados al sistema.\n";
            }
            break;
        }

               
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