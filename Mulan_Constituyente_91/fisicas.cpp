#include "fisicas.h"
#include <cmath>//Calculo de funciones trigonometricas

Cinematica SaltoParabolico::calcular(double t, const Cinematica& s) const {
    Cinematica r;
    //Posicion horizontal. x(t) = x_0+v_{0x}*t
    r.posicion.setX(s.posicion.x() + s.velocidad.x() * t);
    //Posicion vertical. y(t) = y_0 + v_{0y} * t + (1/2)g*t^2
    r.posicion.setY(s.posicion.y() + s.velocidad.y() * t + 0.5 * gravedad * t * t);
    //Velocidad vertical. v_y(t)=v_{0y} + g*t
    r.velocidad = QPointF(s.velocidad.x(), s.velocidad.y() + gravedad * t);
    return r;
}

Cinematica Oscilacion::calcular(double t, const Cinematica& s) const {
    Cinematica r = s;//Copiar para conservar los datos en y
    /*MAS, Posición horizontal. x(t) = x_0 + A*sin(omega*t + phi)*30
    '30':factor de escala para convertir la amplitud en unidades de pantalla*/
    r.posicion.setX(s.posicion.x() + amplitud * std::sin(frecuencia * t + fase) * 30.0);
    //Velocidad horizontal. v_x(t) = 30 * A*omega * cos(omega*t + phi)
    r.velocidad.setX(amplitud * frecuencia * std::cos(frecuencia * t + fase) * 30.0);
    return r;
}

Cinematica MovimientoAmortiguado::calcular(double t, const Cinematica& s) const {
    Cinematica r;
    /*Movimiento con aceleracion constante y resistencia proporcional a la velocidad. dv/dt = a - kv
     e**(-kt) */
    const double e = std::exp(-k * t);
    //Velocidad horizontal inicial
    const double v0 = s.velocidad.x();
    //Calcular la velocidad. v(t) = (v_0 - (a/k)*e^{-kt} + (a/k)
    const double v = (v0 - aceleracion / k) * e + aceleracion / k;
    //Calcular posicion. x(t) = x_0 + (v_0 - (a/k)*[(1-e^{-kt})/k] + (a/k)*t
    const double x = s.posicion.x() + (v0 - aceleracion / k) * (1.0 - e) / k + (aceleracion / k) * t;

    r.posicion = QPointF(x, s.posicion.y());
    r.velocidad = QPointF(v, s.velocidad.y());
    return r;
}

Cinematica CaidaDocumentos::calcular(double t, const Cinematica& s) const {
    Cinematica r;
    //Posición horizontal. x(t)=x_0 + A * sin(omega*t)
    r.posicion.setX(s.posicion.x() + amplitud * std::sin(frecuencia * t));
    //Posición vertical. y(t) = y_0 + v_{0y}*t + (1/2)g*t^2
    r.posicion.setY(s.posicion.y() + s.velocidad.y() * t + 0.5 * gravedad * t * t);
    /*Velocidad horizontal. v_x(t) = A * omega * cos(omega*t)
      Velocidad vertical. v_y(t) = v_{0y} + g*t    */
    r.velocidad = QPointF(amplitud * frecuencia * std::cos(frecuencia * t),
                          s.velocidad.y() + gravedad * t);
    return r;
}

