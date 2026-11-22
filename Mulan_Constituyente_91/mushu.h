#ifndef MUSHU_H
#define MUSHU_H

#include <QString>
#include <QVector>
#include <QHash>//Busqueda más rapida, paresno ordenados

//Información que Mushu necesita conocer para tomar decisiones
struct DatosJuego {
    int documentosPendientes = 0;//Cantidad de documentos que todavía debe recoger Mulán
    int tiempoRestante = 0;//Tiempo disponible para completar el nivel.
    int fallosSalto = 0;//Cantidad de representantes que apoyan la propuesta de Mulán.
    int apoyos = 0;//Cantidad de representantes que apoyan la propuesta de Mulán.
    int representantesPendientes = 0;//Cantidad de representantes que todavía requieren atención.
    QString ultimoEvento; //Último acontecimiento importante, por ejemplo, "objecion" o "apoyo".
};

//Experiencia que Mushu almacena para aprender de las situaciones anteriores
struct Experiencia {
    QString contexto;//Situacion en la que se tomó una decisión
    QString accion;//Acción que seleccionó
    double recompensa = 0.0;
};

/*Componente de percepción. Recibir informacion y construir una representación del estado que pueda
 utilizar el sistema de razonamiento*/
class PercepcionMushu {
public:
    DatosJuego construirEstado(const DatosJuego& observado) const;
};

//Componente de razonamiento. Evaluar la situación actual y considerar lo que Mushu ha aprendido
class RazonamientoMushu {
public:
    QString seleccionarAccion(const DatosJuego& estado, const QHash<QString, double>& valoraciones) const;
};

//Componente de acción. Convertir la decisión del razonamiento en un mensaje para el jugador
class AccionMushu {
public:
    QString ejecutar(const QString& accion) const;
};

/*Componente de aprendizaje. En 'historial' almacena todas las experiencias registradas, con 'valoraciones' almacena
  la valoración numerica asociada a cada acción.*/
class AprendizajeMushu {
private:
    QVector<Experiencia> historial;
    QHash<QString, double> valoraciones;
public:
    //Almacena nueva experiencia y actualiza la valoración de la accion
    void registrar(const QString& contexto, const QString& accion, double recompensa);
    //Devuelve la valoración numerica de una accion
    double valoracion(const QString& accion) const;
    //Consultar los datos internos de aprendizaje
    const QVector<Experiencia>& obtenerHistorial() const { return historial; }
    const QHash<QString, double>& obtenerValoraciones() const { return valoraciones; }
};

//Gestionar funcionamiento de Mushu
class Mushu {
private:
    PercepcionMushu percepcion;
    RazonamientoMushu razonamiento;
    AccionMushu accion;
    AprendizajeMushu aprendizaje;
    QString ultimaAccion;//ultima acción seleccionada por Mushu
public:
    QString observarActuar(const DatosJuego& estado);
    void aprenderResultado(const QString& contexto, double recompensa);
    const QVector<Experiencia>& historial() const { return aprendizaje.obtenerHistorial(); }
};
#endif // MUSHU_H
