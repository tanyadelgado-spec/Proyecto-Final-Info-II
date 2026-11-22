#ifndef FISICAS_H
#define FISICAS_H


#include <QPointF>//Representar puntos/vectores bidimensionales en coordenadas 'double'

//Estado físico de un objeto en un instante t
struct Cinematica {
    QPointF posicion;
    QPointF velocidad;
};

//Interfaz comun para todos los modelos de movimiento
class Fisica{
public:
    virtual ~Fisica() = default;
    virtual Cinematica calcular(double t, const Cinematica& inicial) const = 0;
};

//Movimiento parabólico de un objeto sometido a la gravedad
class SaltoParabolico : public Fisica {
private:
    //900 unidades/s**2, con eje Y creciendo hacia abajo, la gravedad es +
    double gravedad;

public:
    explicit SaltoParabolico(double g = 900.0) : gravedad(g) {}
    //Ecuaciones del movimiento rectilneo uniformemente acelerado
    Cinematica calcular(double t, const Cinematica& s) const override;
};

//Oscilacion angular de objetos colgantes
class Oscilacion : public Fisica {
private:
    //Características de la oscilación
    double amplitud, frecuencia, fase;

public:
    Oscilacion(double a = 0.45, double w = 2.0, double p = 0.0) : amplitud(a), frecuencia(w), fase(p) {}
    //Ecuaciones del movimiento armonico simple
    Cinematica calcular(double t, const Cinematica& s) const override;
};

// Movimiento con aceleracion externa y amortiguamiento lineal
class MovimientoAmortiguado : public Fisica {
private:
    //'k'. Coeficiente de amortiguamiento s**(-1)
    double aceleracion, k;

public:
    MovimientoAmortiguado(double a = 300.0, double amortiguamiento = 2.0) : aceleracion(a), k(amortiguamiento) {}
    //Ecuación diferencial para describir movimiento con aceleración constante y resistencia proporcional a la velocidad
    Cinematica calcular(double t, const Cinematica& s) const override;
};

// Caida de documentos con oscilacion horizontal
class CaidaDocumentos : public Fisica {
private:
    double gravedad, amplitud, frecuencia;
public:
    CaidaDocumentos(double g = 500.0, double a = 18.0, double w = 3.0) : gravedad(g), amplitud(a), frecuencia(w) {}
    /*Ecuaciones de movimiento oscilatorio horizontal y MUA vertical.*/
    Cinematica calcular(double t, const Cinematica& s) const override;
};

#endif // FISICAS_H
