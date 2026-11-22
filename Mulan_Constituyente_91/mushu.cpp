#include "mushu.h"

//Estado de juego actual
DatosJuego PercepcionMushu::construirEstado(const DatosJuego& observado) const {
    return observado;
}

//Analiza el estado del juego y selecciona recomendacion
QString RazonamientoMushu::seleccionarAccion(
    const DatosJuego& e, const QHash<QString, double>& valores) const {
    QString opcion = "pista_general";//accion predeterminada

    //Si Mulan ha cometido 2 o mas errores al saltar, Mushu recomienda utilizar una ruta diferente.
    if (e.fallosSalto >= 2) opcion = "ruta_alternativa";
    //Si quedan 25s o menos y todavía hay documentos pendientes, recomienda priorizar su recoleccion
    else if (e.tiempoRestante <= 25 && e.documentosPendientes > 0)
        opcion = "priorizar_documento";
    //Si hay representantes pendientes y el ultimo evento fue una 'objecion', recomienda buscar una evidencia relacionada
    else if (e.representantesPendientes > 0 && e.ultimoEvento == "objecion")
        opcion = "buscar_evidencia";
    //Si se ha conseguido 4 o mas apoyos, Mushu reconoce que se ha alcanzado el objetivo de la asamblea
    else if (e.apoyos >= 4) opcion = "acuerdo_alcanzado";

    //La valoracion aprendida favorece una alternativa conocida
    if (valores.value("ruta_alternativa", 0.0) > valores.value(opcion, 0.0) + 2.0 && e.fallosSalto > 0)
        opcion = "ruta_alternativa";
    return opcion;
}

//Recibe la acción seleccionada por el 'razonamiento' devolviendo mensaje correspondiente
QString AccionMushu::ejecutar(const QString& a) const {
    if (a == "ruta_alternativa") return "Intenta una plataforma intermedia o una ruta distinta.";
    if (a == "priorizar_documento") return "El tiempo es limitado; busca primero el documento mas cercano.";
    if (a == "buscar_evidencia") return "Presenta una evidencia relacionada con la objecion.";
    if (a == "acuerdo_alcanzado") return "Ya hay apoyo suficiente. ¡Buen trabajo!";
    return "Analizando el entorno";
}

//Permite que Mushu modifique sus valoraciones a partir de las experiencias registradas
void AprendizajeMushu::registrar(const QString& contexto, const QString& a, double recompensa) {
    //Agrega la nueva experiencia al final del vector 'historial'
    historial.push_back({contexto, a, recompensa});
    //Cte = 0.30, controla cuánto influye la nueva recompensa sobre la valoración anterior
    const double var = 0.30;
    const double anterior = valoraciones.value(a, 0.0);//Consultar valoracion anterior de la accion
    //Ecuacion de aprendizaje. V_{nuevo} = (1-var) * V_{anterior}+ var * R
    valoraciones[a] = (1.0 - var) * anterior + var * recompensa;
}

//Valoracion aprendida de una accion
double AprendizajeMushu::valoracion(const QString& a) const {
    return valoraciones.value(a, 0.0);
}

//Integrar los 4 componentes para el ciclo principal de funcionamiento de Mushu
QString Mushu::observarActuar(const DatosJuego& estado) {
    //Recibir informacion del juego y construir estado a utilizar
    DatosJuego percibido = percepcion.construirEstado(estado);
    //Analizar estado y seleccionar una accion.
    ultimaAccion = razonamiento.seleccionarAccion(percibido, aprendizaje.obtenerValoraciones());

    return accion.ejecutar(ultimaAccion);//Generacion del mensaje a mostrar al jugador
}

//Mushu aprenda despues de haber recomendado una accion
void Mushu::aprenderResultado(const QString& contexto, double recompensa) {
    if (!ultimaAccion.isEmpty())//Si ya se ha seleccionado una accion
        aprendizaje.registrar(contexto, ultimaAccion, recompensa);
}
