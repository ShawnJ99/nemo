// ============================================================================
// Copyright Jean-Charles LAMBERT - 2007-2010                                  
// e-mail:   Jean-Charles.Lambert@oamp.fr                                      
// address:  Dynamique des galaxies                                            
//           Laboratoire d'Astrophysique de Marseille                          
//           P�le de l'Etoile, site de Ch�teau-Gombert                         
//           38, rue Fr�d�ric Joliot-Curie                                     
//           13388 Marseille cedex 13 France                                   
//           CNRS U.M.R 6110                                                   
// ============================================================================
// See the complete license in LICENSE and/or "http://www.cecill.info".        
// ============================================================================
/**
	@author Jean-Charles Lambert <Jean-Charles.Lambert@oamp.fr>
*/
#ifndef GLNEMOGLGRIDOBJECT_H
#define GLNEMOGLGRIDOBJECT_H
#include <QGLWidget>

#include "globject.h"
#include "glgridobject.h"

namespace glnemo {


class GLGridObject: public GLObject {
public:

  GLGridObject(int axe_parm=0,const QColor &c=Qt::yellow, bool activated=TRUE);
  
  ~GLGridObject();

  // method
  void setNbSquare(int _nsquare) { nsquare = _nsquare;};
  void setSquareSize(float _square_size) { square_size = _square_size;};
  void rebuild() { buildDisplayList(); };
 private:
  static int nsquare;        // #square       
  static float square_size;  // size of square
  int axe;
  // method
  void  buildDisplayList( );
};

}

#endif
