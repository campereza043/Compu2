/* 

#ifndef VECTOR3D_MIN_HPP
#define VECTOR3D_MIN_HPP

#include <iostream>
#include <cmath>

using namespace std;
*/

#pragma once
#include <iostream>
#include <cmath>

//---------------------- class vector3D (versión mínima) --------------------
class vector3D{
  double v[3];
public:
  // Carga y visualización
  void   cargue(double x0, double y0, double z0){ v[0]=x0; v[1]=y0; v[2]=z0; }
  void   show(void) const { std::cout << "(" << v[0] << "," << v[1] << "," << v[2] << ")\n"; }

  // Getters de componentes
  double x(void) const { return v[0]; }
  double y(void) const { return v[1]; }
  double z(void) const { return v[2]; }

  // Indexación (lectura/escritura)
  double & operator[](int i){ return v[i]; }
  double   operator[](int i) const { return v[i]; }

  // Asignación
  vector3D operator=(vector3D v2){
    for(int i=0;i<3;i++) v[i] = v2.v[i];
    return *this;
  }

  // Suma y resta
  vector3D operator+(vector3D v2){
    vector3D t; t.v[0]=v[0]+v2.v[0]; t.v[1]=v[1]+v2.v[1]; t.v[2]=v[2]+v2.v[2]; return t;
  }
  vector3D operator+=(vector3D v2){
    *this = *this + v2; return *this;
  }
  vector3D operator-(vector3D v2){
    vector3D t; t.v[0]=v[0]-v2.v[0]; t.v[1]=v[1]-v2.v[1]; t.v[2]=v[2]-v2.v[2]; return t;
  }
  vector3D operator-=(vector3D v2){
    *this = *this - v2; return *this;
  }
  vector3D operator-() const { vector3D t; t.cargue(-v[0],-v[1],-v[2]); return t; }

  // Producto punto (entre vectores)
  double operator*(vector3D v2){
    return v[0]*v2.v[0] + v[1]*v2.v[1] + v[2]*v2.v[2];
  }

  // Producto cruz (entre vectores)
  vector3D operator^(vector3D v2){
    vector3D c;
    c.cargue( v[1]*v2.v[2]-v[2]*v2.v[1],
              v[2]*v2.v[0]-v[0]*v2.v[2],
              v[0]*v2.v[1]-v[1]*v2.v[0] );
    return c;
  }

  // Normas (ojo con el v[1] -> v1.v[1])
  friend double norma2(vector3D v1){
    return v1.v[0]*v1.v[0] + v1.v[1]*v1.v[1] + v1.v[2]*v1.v[2];
  }
  friend double norma(vector3D v1){
    return std::sqrt(norma2(v1));
  }
};
