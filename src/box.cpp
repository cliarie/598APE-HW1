#include "box.h"

Box::Box(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty):Plane(c, t, ya, pi, ro, tx, ty){}
Box::Box(const Vector &c, Texture* t, double ya, double pi, double ro, double tx):Plane(c, t, ya, pi, ro, tx,tx){}

inline double Box::getIntersection(Ray ray){
   double time = Plane::getIntersection(ray);
   if (time == inf) return time;
   Vector hitpoint = ray.point+(ray.vector*time)-center;
   Vector dist = solveScalers(right, up, vect, hitpoint);
   return (fabs(dist.x)> textureX * 0.5 || fabs(dist.y)>textureY * 0.5 )?inf:time;
}

inline bool Box::getLightIntersection(Ray ray, double* fill){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;
   if(r<=0. || r>=1.) return false;
   Vector hitpoint = ray.point+ray.vector*r-center;
   Vector dist = solveScalers(right, up, vect, hitpoint);
   if (fabs(dist.x)>textureX * 0.5 || fabs(dist.y)>textureY * 0.5) return false;

   if(texture->opacity>1-1E-6) return true;   
   unsigned char temp[4];
   double amb, op, ref;
   texture->getColor(temp, &amb, &op, &ref,fix(dist.x/textureX-.5), fix(dist.y/textureY-.5));
   if(op>1-1E-6) return true;
   fill[0]*=temp[0]/255.;
   fill[1]*=temp[1]/255.;
   fill[2]*=temp[2]/255.;
   return false;
}
