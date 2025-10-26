#ifndef CAJA_H
#define CAJA_H

class Caja {
private:
    double W, H; // ancho y alto
public:
    Caja();                         // constructor por defecto
    void Defina(double W_, double H_); // definir dimensiones
    double GetW() const { return W; }
    double GetH() const { return H; }
};

#endif
