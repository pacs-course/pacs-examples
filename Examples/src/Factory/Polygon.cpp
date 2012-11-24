#include "Polygon.hpp"
#include <iostream>

namespace Geometry{
  
  // ********************* CLASSE BASE **********************
  Polygon::Polygon(bool convex):isconvex(convex),vertexes(){}
  Vertices const & Polygon::theVertices()const{return vertexes;};
  Polygon::Polygon(Vertices v, bool convex):isconvex(convex),vertexes(v){}
  Polygon::Polygon(Polygon const & p):isconvex(p.isconvex),vertexes(p.vertexes){}
  void Polygon::setVertices(Vertices const & v){
    vertexes=v;
  }
  Polygon::~Polygon(){}
  bool Polygon::isConvex() const {return isconvex;}
  void Polygon::showMe(ostream & out)const
  {out<<"Poligono Base"<<std::endl;}


  double Polygon::inscibedCircle() const{
    // to be implemented
    return 0;
  }  
  double Polygon::diameter() const{
    // to be implemented
    return 0;
  }
  void Polygon::translate(double const & x, double const & y){
    // to be implemented
  }
  void Polygon::rotate(double const & angle){
    // to be implemented
  }
  void Polygon::scale(double const & xfactor, double const & yfactor){
    // to be implemented
  }

  
  // ********************* QUADRILATERO **********************
  
  Quadrilateral::Quadrilateral():Polygon(){};
  Quadrilateral::Quadrilateral(Vertices const & v):Polygon(v){
    // codice per deteminare la convessita` a n. vertici
    ;}
  Quadrilateral::Quadrilateral(Vertices const & v,bool c):Polygon(v,c){}
  
  Quadrilateral::Quadrilateral(Quadrilateral const & q):Polygon(q){}
  
  Quadrilateral::~Quadrilateral(){}
  
  void Quadrilateral::showMe(ostream & out) const
  {out<<"I have four sides, like any QUADRILATERAL should"<<std::endl;}
  
  

  // ********************* TRIANGOLO **********************
  
  Triangle::Triangle():Polygon(){};
  Triangle::Triangle(Vertices const & v):Polygon(v,true){
    // codice di controllo
    ;}
  
  Triangle::Triangle(Triangle const & q):Polygon(q){}
  
  Triangle::~Triangle(){}
  
  void Triangle::showMe(ostream & out) const
  {out<<"The eternal TRIANGLE"<<std::endl;}
  
  
  // ********************* QUADRATO **********************
  
  Square::Square():Quadrilateral(){};
  Square::Square(Vertices const & v):Quadrilateral(v,true){
    isconvex=true;
    ;}
  
  Square::Square(Square const & s):Quadrilateral(s.vertexes,true){}
  
  
  void Square::showMe(ostream & out) const
  {out<<"I am rather SQUARE, what about you?"<<std::endl;}
  
  double Square::diagLen()const{
      //metodo per calcolare la diagonale
      return 0;};
}

