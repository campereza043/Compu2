#include "vector.h"
#include <iostream>   // importante

int main(){
  vector3D a,b,c;
  a.cargue(1,2,3);
  b.cargue(4,5,6);

  c = a + b;   c.show();        
  c = a - b;   c.show();        
  std::cout << "a·b = " << (a*b) << "\n";        
  vector3D d = a ^ b; d.show();            
  std::cout << "|a| = " << norma(a) << "\n";     
  return 0;
}
