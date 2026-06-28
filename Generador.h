#ifndef GENERADOR_H
#define GENERADOR_H

#include "Entidades.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;

class Generador {
private:
    vector<string> nombresBase = {
        "carlos","maria","jose","ana","luis","sofia","diego","valeria",
        "miguel","daniela","fernando","camila","andres","isabella",
        "ricardo","gabriela","roberto","natalia","eduardo","paola",
        "pedro","lucia","marco","andrea","julian","valentina","sebastian"
    };
    vector<string> tiposSus = { "Gratis","Plus","Premium","Familiar" };
    vector<string> temasLec = {
        "Saludos basicos","Numeros","Colores","Animales","Familia",
        "Comida","Viajes","Trabajo","Salud","Tiempo libre",
        "Transporte","Tecnologia","Naturaleza","Deportes","Musica"
    };
    vector<string> palabrasEN = {
        "hello","water","book","house","cat","run","eat","friend",
        "happy","work","school","time","love","music","sky"
    };
    vector<string> traducciones = {
        "hola","agua","libro","casa","gato","correr","comer","amigo",
        "feliz","trabajo","escuela","tiempo","amor","musica","cielo"
    };
    vector<string> nombresLogro = {
        "Primera leccion","Racha 7 dias","100 puntos",
        "Poliglota","Relampago","Maestro del mes","1000 palabras"
    };
    vector<string> nombresItem = {
        "Avatar Dino","Tema Noche","Boost XP x2","Escudo Racha",
        "Avatar Robot","Tema Galaxy","Marco Dorado","Streak Freeze"
    };
    vector<string> mensajesNotif = {
        "Tu racha esta en peligro!","Nueva leccion disponible.",
        "Has desbloqueado un logro!","Practica tu leccion diaria.",
        "Tu amigo te supero en puntos!","Completaste la mision diaria!"
    };

    int aleatorio(int minV, int maxV) const {
        if (maxV <= minV) return minV;
        return minV + rand() % (maxV - minV + 1);
    }
    template<typename T>
    const T& elem(const vector<T>& v) const { return v[aleatorio(0, (int)v.size() - 1)]; }

    string generarFecha(int aMin = 2025, int aMax = 2026) const {
        ostringstream oss;
        oss << aleatorio(aMin, aMax) << "-"
            << setw(2) << setfill('0') << aleatorio(1, 12) << "-"
            << setw(2) << setfill('0') << aleatorio(1, 28);
        return oss.str();
    }

public:
    Generador() { srand((unsigned)time(nullptr)); }
    explicit Generador(unsigned s) { srand(s); }

    
    vector<Usuario> generarUsuarios(int n = 20) const {
        vector<Usuario> lista;
        for (int i = 1; i <= n; i++) {
            Usuario u;
            u.nombre = elem(nombresBase) + to_string(i);
            u.puntos = aleatorio(0, 500);
            u.suscripcion = elem(tiposSus);
            lista.push_back(u);
        }
        return lista;
    }

    
    vector<Idioma> generarIdiomas() const {
        return {
            {"Ingles",    "Facil"   },
            {"Frances",   "Medio"   },
            {"Aleman",    "Medio"   },
            {"Japones",   "Dificil" },
            {"Chino",     "Dificil" },
            {"Portugues", "Facil"   },
            {"Italiano",  "Medio"   },
            {"Coreano",   "Dificil" },
            {"Ruso",      "Dificil" },
            {"Arabe",     "Dificil" }
        };
    }

    
    vector<Curso> generarCursos(int n = 8) const {
        vector<string> idiomaNombres = {
            "Ingles","Frances","Aleman","Japones","Chino","Portugues"
        };
        vector<Curso> lista;
        for (int i = 0; i < n; i++) {
            Curso c;
            c.nombre_idioma = idiomaNombres[i % (int)idiomaNombres.size()];
            c.nivel_actual = aleatorio(1, 5);
            lista.push_back(c);
        }
        return lista;
    }

    
    vector<Leccion> generarLecciones(int n = 20) const {
        vector<Leccion> lista;
        for (int i = 0; i < n; i++) {
            Leccion l;
            l.titulo = temasLec[i % (int)temasLec.size()];
            l.duracion = aleatorio(5, 30);
            lista.push_back(l);
        }
        return lista;
    }

    
    vector<Vocabulario> generarVocabulario(int n = 30) const {
        vector<Vocabulario> lista;
        for (int i = 0; i < n; i++) {
            int idx = i % (int)palabrasEN.size();
            lista.push_back({ palabrasEN[idx], traducciones[idx] });
        }
        return lista;
    }

    
    vector<Progreso> generarProgreso(int n = 20) const {
        vector<Progreso> lista;
        for (int i = 0; i < n; i++) {
            Progreso p;
            p.fecha = generarFecha();
            p.lecciones_completadas = aleatorio(0, 80);
            lista.push_back(p);
        }
        return lista;
    }

    
    vector<Logro> generarLogros() const {
        vector<Logro> lista;
        vector<string> descs = {
            "Completa tu primera leccion",
            "Mantén racha 7 dias consecutivos",
            "Acumula 100 puntos",
            "Aprende 3 idiomas distintos",
            "Completa leccion en menos de 1 min",
            "Racha de 30 dias",
            "Aprende 1000 palabras"
        };
        for (int i = 0; i < (int)nombresLogro.size(); i++)
            lista.push_back({ nombresLogro[i], descs[i] });
        return lista;
    }

    // generarExamenes
    vector<Examen> generarExamenes(int n = 10) const {
        vector<Examen> lista;
        for (int i = 0; i < n; i++) {
            Examen e;
            e.tema = "Examen " + temasLec[i % (int)temasLec.size()];
            e.nota = aleatorio(0, 20);
            lista.push_back(e);
        }
        return lista;
    }

    
    // generarPreguntas
    
    vector<Pregunta> generarPreguntas(int n = 20) const {
        vector<string> enunc = {
            "Como se dice 'hello'?","Traduce 'water'","Plural de 'mouse'",
            "Forma pasada de 'go'","Articulo de 'apple'"
        };
        vector<string> resp = { "hola","agua","mice","went","an" };
        vector<Pregunta> lista;
        for (int i = 0; i < n; i++)
            lista.push_back({ enunc[i % (int)enunc.size()], resp[i % (int)resp.size()] });
        return lista;
    }

    
    // generarRanking
    vector<Ranking> generarRanking(int n = 10) const {
        vector<Ranking> lista;
        for (int i = 1; i <= n; i++) {
            Ranking r;
            r.usuario = elem(nombresBase) + to_string(i);
            r.posicion = aleatorio(10, 1000);
            lista.push_back(r);
        }
        return lista;
    }

    
    // generarAmigos
    vector<Amigo> generarAmigos(int n = 10) const {
        vector<Amigo> lista;
        for (int i = 0; i < n; i++) {
            Amigo a;
            a.nombre = elem(nombresBase) + to_string(i + 1);
            a.conectado = (aleatorio(0, 1) == 1);
            lista.push_back(a);
        }
        return lista;
    }

   
    // generarNotificaciones
    vector<Notificacion> generarNotificaciones(int n = 10) const {
        vector<string> horas = {
            "08:00","09:30","12:00","15:00","18:30","20:00","22:00"
        };
        vector<Notificacion> lista;
        for (int i = 0; i < n; i++) {
            Notificacion nt;
            nt.mensaje = mensajesNotif[i % (int)mensajesNotif.size()];
            nt.hora = horas[i % (int)horas.size()];
            lista.push_back(nt);
        }
        return lista;
    }

    
    // generarItems
    vector<Item> generarItems() const {
        vector<Item> lista;
        for (int i = 0; i < (int)nombresItem.size(); i++) {
            Item it;
            it.tipo = nombresItem[i];
            it.desbloqueado = (i % 3 == 0); // algunos ya desbloqueados
            lista.push_back(it);
        }
        return lista;
    }

    
    // generarTienda
    vector<Tienda> generarTienda() const {
        int costos[] = { 200,150,300,100,350,250,500,400 };
        vector<Tienda> lista;
        for (int i = 0; i < (int)nombresItem.size(); i++) {
            Tienda t;
            t.nombre_item = nombresItem[i];
            t.costo = costos[i];
            lista.push_back(t);
        }
        return lista;
    }

    
    // generarSuscripciones
    vector<Suscripcion> generarSuscripciones() const {
        return {
            {"Gratis",   0.0f  },
            {"Plus",     6.99f },
            {"Premium",  13.99f},
            {"Familiar", 19.99f}
        };
    }

    
    // Guardar/cargar archivos
    void guardarUsuariosEnArchivo(const vector<Usuario>& usuarios,
        const string& archivo = "usuarios.txt") const {
        ofstream f(archivo);
        if (!f.is_open()) { cerr << "  Error al abrir " << archivo << "\n"; return; }
        for (auto& u : usuarios)
            f << u.nombre << " " << u.puntos << " " << u.suscripcion << "\n";
        f.close();
        cout << "  [OK] " << usuarios.size() << " usuarios guardados en " << archivo << "\n";
    }

    vector<Usuario> cargarUsuariosDesdeArchivo(const string& archivo = "usuarios.txt") const {
        vector<Usuario> lista;
        ifstream f(archivo);
        if (!f.is_open()) { cerr << "  Error al abrir " << archivo << "\n"; return lista; }
        string n, s; int p;
        while (f >> n >> p >> s) lista.push_back({ n, p, s });
        f.close();
        cout << "  [OK] " << lista.size() << " usuarios cargados desde " << archivo << "\n";
        return lista;
    }

    void guardarLeccionesEnArchivo(const vector<Leccion>& lecciones,
        const string& archivo = "lecciones.txt") const {
        ofstream f(archivo);
        if (!f.is_open()) return;
        for (auto& l : lecciones)
            f << l.titulo << "|" << l.duracion << "\n";
        f.close();
        cout << "  [OK] " << lecciones.size() << " lecciones guardadas en " << archivo << "\n";
    }

    void guardarVocabularioEnArchivo(const vector<Vocabulario>& vocab,
        const string& archivo = "vocabulario.txt") const {
        ofstream f(archivo);
        if (!f.is_open()) return;
        for (auto& v : vocab)
            f << v.palabra << "|" << v.traduccion << "\n";
        f.close();
        cout << "  [OK] " << vocab.size() << " palabras guardadas en " << archivo << "\n";
    }

    void generarTodo(int nUsuarios = 20, int nLecciones = 20, int nVocab = 30) const {
        cout << "\n  === Generando dataset completo ===\n";
        guardarUsuariosEnArchivo(generarUsuarios(nUsuarios));
        guardarLeccionesEnArchivo(generarLecciones(nLecciones));
        guardarVocabularioEnArchivo(generarVocabulario(nVocab));
        cout << "  === Dataset listo ===\n\n";
    }
};
#endif