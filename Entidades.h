#ifndef ENTIDADES_H
#define ENTIDADES_H
#include <string>
#include <vector>

using namespace std;

struct Usuario { string nombre; int puntos; string suscripcion; };
struct Idioma { string nombre; string dificultad; };
struct Curso { string nombre_idioma; int nivel_actual; };
struct Leccion { string titulo; int duracion; };
struct Vocabulario { string palabra; string traduccion; };
struct Progreso { string fecha; int lecciones_completadas; };
struct Logro { string nombre; string descripcion; };
struct Examen { string tema; int nota; };
struct Pregunta { string enunciado; string respuesta; };
struct Ranking { string usuario; int posicion; };
struct Amigo { string nombre; bool conectado; };
struct Notificacion { string mensaje; string hora; };
struct Tienda { string nombre_item; int costo; };
struct Item { string tipo; bool desbloqueado; };
struct Suscripcion { string tipo; float precio; };

#endif
