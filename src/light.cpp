
#include "light.h"
#include "shape.h"
#include "camera.h"
#include <algorithm>
#include <memory>
#include <utility>
      
Light::Light(const Vector & cente, unsigned char* colo) : center(cente){
   color = colo;
}

unsigned char* Light::getColor(unsigned char a, unsigned char b, unsigned char c){
   unsigned char* r = (unsigned char*)malloc(sizeof(unsigned char)*3);
   r[0] = a;
   r[1] = b;
   r[2] = c;
   return r;
}

Autonoma::Autonoma(const Camera& c): camera(c){
   listStart = NULL;
   listEnd = NULL;
   lightStart = NULL;
   lightEnd = NULL;
   depth = 10;
   skybox = BLACK;
}

Autonoma::Autonoma(const Camera& c, Texture* tex): camera(c){
   listStart = NULL;
   listEnd = NULL;
   lightStart = NULL;
   lightEnd = NULL;
   depth = 10;
   skybox = tex;
}

void Autonoma::addShape(std::shared_ptr<Shape> r) {
   std::unique_ptr<ShapeNode> hi = std::make_unique<ShapeNode>();
   hi->data = r;
   hi->next = nullptr;
   hi->prev = nullptr;
   
   if (listStart == nullptr) {
      listStart = std::move(hi);
      listEnd = listStart.get();
   } else {
      listEnd->next = std::move(hi);
      if (listEnd->next) {
         listEnd->next->prev = listEnd;
         listEnd = listEnd->next.get();
      } else {
         fprintf(stderr, "Error: failed to add shape, new node is null.\n");
         exit(1);
      }
   }
}

void Autonoma::removeShape(ShapeNode* s){
   if (!s) return;
   if(s==listStart.get()){
      if(s==listEnd){
         listStart.reset();
         listEnd = NULL;
      }
      else{
         listStart = std::move(s->next);
         listStart->prev = NULL;
      }
   }
   else if(s==listEnd){
      listEnd = s->prev;
      listEnd->next.reset();
   }
   else{
      s->prev->next = std::move(s->next);
      if (s->prev->next) s->prev->next->prev = s->prev;
   }
}

void Autonoma::addLight(std::shared_ptr<Light> r) {
   std::unique_ptr<LightNode> hi = std::make_unique<LightNode>();
   hi->data = r;
   hi->next = nullptr;
   hi->prev = nullptr;
   
   if (lightStart == nullptr) {
      lightStart = std::move(hi);
      lightEnd = lightStart.get();
   } else {
      lightEnd->next = std::move(hi);
      lightEnd->next->prev = lightEnd;
      lightEnd = lightEnd->next.get();
   }
}

void Autonoma::removeLight(LightNode* s){
   if (!s) return;
   if(s==lightStart.get()){
      if(s==lightEnd){
         lightStart.reset();
         lightEnd = NULL;
      }
      else{
         lightStart = std::move(s->next);
         lightStart->prev = NULL;
      }
   }
   else if(s==lightEnd){
      lightEnd = s->prev;
      lightEnd->next.reset();
   }
   else{
      s->prev->next = std::move(s->next);
      if (s->prev->next) s->prev->next->prev = s->prev;
   }
}

void getLight(double* tColor, Autonoma* aut, Vector point, Vector norm, unsigned char flip){
   tColor[0] = tColor[1] = tColor[2] = 0.;
   LightNode *t = aut->lightStart.get();
   while(t!=NULL){
      double lightColor[3];     
      lightColor[0] = t->data->color[0]/255.;
      lightColor[1] = t->data->color[1]/255.;
      lightColor[2] = t->data->color[2]/255.;
      Vector ra = t->data->center-point;
      ShapeNode* shapeIter = aut->listStart.get();
      bool hit = false;
      while(!hit && shapeIter!=NULL){
        hit = shapeIter->data->getLightIntersection(Ray(point+ra*.01, ra), lightColor);
         shapeIter = shapeIter->next.get();
      }
      double perc = (norm.dot(ra)/(ra.mag()*norm.mag()));
      if(!hit){
      if(flip && perc<0) perc=-perc;
        if(perc>0){
      
         tColor[0]+= perc*(lightColor[0]);
         tColor[1]+= perc*(lightColor[0]);
         tColor[2]+= perc*(lightColor[0]);
         if(tColor[0]>1.) tColor[0] = 1.;
         if(tColor[1]>1.) tColor[1] = 1.;
         if(tColor[2]>1.) tColor[2] = 1.;
        }
      }
      t =t->next.get();
   }
}
