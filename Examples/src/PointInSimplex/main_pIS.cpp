#include <iostream>
#include "PointInSimplex.hpp"
int main()
{

    //Creating a triangle
    apsc::PointInS_Traits::Triangle t;
    t<<apsc::PointInS_Traits::Point2D{0.0,0.0}.transpose(),
        apsc::PointInS_Traits::Point2D{1.0,0.0}.transpose(),
        apsc::PointInS_Traits::Point2D{0.0,1.0}.transpose();
    //Creating a point
    apsc::PointInS_Traits::Point2D p;
    //Checking if the point is inside the triangle
    std::cout<<"Insert the coordinates of the 2D point\n";
    std::cin>>p[0]>>p[1];
    auto [in,lambda]=apsc::pointInTriangle(p,t);
    std::cout<<"The point is "<<(in?"":"not ")<<"inside the triangle\n";
    std::cout<<"The baricentric coordinates are "<<lambda[0]<<", "<<lambda[1]<<", "<<lambda[2]<<'\n';
    //Creating a tetrahedron
    apsc::PointInS_Traits::Tetrahedron tet;
    tet<<apsc::PointInS_Traits::Point3D{0.0,0.0,0.0}.transpose(),
        apsc::PointInS_Traits::Point3D{1.0,0.0,0.0}.transpose(),    
        apsc::PointInS_Traits::Point3D{0.0,1.0,0.0}.transpose(),
        apsc::PointInS_Traits::Point3D{0.0,0.0,1.0}.transpose();
    //Creating a point
    apsc::PointInS_Traits::Point3D p3;
    //Checking if the point is inside the tetrahedron
    std::cout<<"Insert the coordinates of the 3D point\n";
    std::cin>>p3[0]>>p3[1]>>p3[2];
    auto [in3,lambda3]=apsc::pointInTetrahedron(p3,tet);
    std::cout<<"The point is "<<(in3?"":"not ")<<"inside the tetrahedron\n";
    std::cout<<"The baricentric coordinates are "<<lambda3[0]<<", "<<lambda3[1]<<", "<<lambda3[2]<<", "<<lambda3[3]<<'\n';
    // Now a triangle in 3D! 
    apsc::PointInS_Traits::Triangle3D t3;
    t3<<apsc::PointInS_Traits::Point3D{0.0,0.0,0.0}.transpose(),
        apsc::PointInS_Traits::Point3D{1.0,0.0,0.0}.transpose(),
        apsc::PointInS_Traits::Point3D{0.0,1.0,0.0}.transpose();
    //Checking if the point is inside the triangle
    std::cout<<"Insert the coordinates of the 3D point\n";
    std::cin>>p3[0]>>p3[1]>>p3[2];
    auto [in3D,lambda3D]=apsc::pointInTriangle3(p3,t3);
    std::cout<<"The point is "<<(in3D?"":"not ")<<"inside the triangle\n";
    std::cout<<"The baricentric coordinates are "<<lambda3D[0]<<", "<<lambda3D[1]<<", "<<lambda3D[2]<<'\n';   

    return 0;
}