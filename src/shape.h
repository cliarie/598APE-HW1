#ifndef __SHAPE_H__
#define __SHAPE_H__
#include "light.h"

class Shape{
  public:
   Shape(const Vector &c, Texture* t, double ya, double pi, double ro);
   virtual ~Shape() {};
   double yaw, pitch, roll, xsin, xcos, ysin, ycos, zsin, zcos;
   Vector center;
   Texture* texture;
   double textureX, textureY, mapX, mapY, mapOffX, mapOffY;
   Texture* normalMap;
   virtual double getIntersection(Ray ray) = 0;
   virtual bool getLightIntersection(Ray ray, double* fill) = 0;
   virtual void move() = 0;
   virtual unsigned char reversible() = 0;
   virtual void getColor(unsigned char* toFill, double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth) = 0;
   virtual Vector getNormal(Vector point) = 0;
   virtual void setAngles(double yaw, double pitch, double roll) = 0;
   virtual void setYaw(double d) = 0;
   virtual void setPitch(double d) = 0;
   virtual void setRoll(double d) = 0;
};


inline void calcColor(unsigned char* toFill,Autonoma* c, Ray&& ray, unsigned int depth){
   double minTime = inf;
   Shape* hitShape = nullptr;
   for (ShapeNode* node = c->listStart.get(); node != nullptr; node = node->next.get()){
      double t = node->data->getIntersection(ray);
      if (t < minTime){
         minTime = t;
         hitShape = node->data.get();
      }
   }
  
   if (!hitShape || minTime == inf){
      double opacity, reflection, ambient;
      Vector temp = ray.vector.normalize();
      double x = temp.x;
      double z = temp.z;
      double me = std::fabs(temp.y);
      double angle = atan2(z, x);
      c->skybox->getColor(toFill, &ambient, &opacity, &reflection, fix(angle/M_TWO_PI), fix(me));
      return;
   }
  
   Vector intersect = minTime*ray.vector+ray.point;
   double opacity, reflection, ambient;
   hitShape->getColor(toFill, &ambient, &opacity, &reflection, c, Ray(intersect, ray.vector), depth);
   
   double lightData[3];
   Vector normal = hitShape->getNormal(intersect);
   getLight(lightData, c, intersect, normal, hitShape->reversible());
   toFill[0] = static_cast<unsigned char>(toFill[0]*(ambient+lightData[0]*(1-ambient)));
   toFill[1] = static_cast<unsigned char>(toFill[1]*(ambient+lightData[1]*(1-ambient)));
   toFill[2] = static_cast<unsigned char>(toFill[2]*(ambient+lightData[2]*(1-ambient)));
   if(depth<c->depth && (opacity<1-1e-6 || reflection>1e-6)){
      unsigned char col[4];
      if(opacity<1-1e-6){
         Ray nextRay = Ray(intersect+ray.vector*1E-4, ray.vector);
         calcColor(col, c, std::move(nextRay), depth+1);
         toFill[0]= static_cast<unsigned char>(toFill[0]*opacity+col[0]*(1-opacity));
         toFill[1]= static_cast<unsigned char>(toFill[1]*opacity+col[1]*(1-opacity));
         toFill[2]= static_cast<unsigned char>(toFill[2]*opacity+col[2]*(1-opacity));        
      }
      if(reflection>1e-6){
         Vector norm = normal.normalize();
         Vector vec = ray.vector-2*norm*(norm.dot(ray.vector));
         Ray nextRay = Ray(intersect+vec*1E-4, vec);
         calcColor(col, c, std::move(nextRay), depth+1);
      
         toFill[0]= static_cast<unsigned char>(toFill[0]*(1-reflection)+col[0]*(reflection));
         toFill[1]= static_cast<unsigned char>(toFill[1]*(1-reflection)+col[1]*(reflection));
         toFill[2]= static_cast<unsigned char>(toFill[2]*(1-reflection)+col[2]*(reflection));
      }
   }
}

#endif
