#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "vector.h"
#include "camera.h"
#include "Textures/texture.h"
#include "Textures/colortexture.h"
#include <memory>

class Light{
  public:
   unsigned char* color;
   unsigned char* getColor(unsigned char a, unsigned char b, unsigned char c);
   Vector center;
   Light(const Vector & cente, unsigned char* colo);
};

struct LightNode{
  std::unique_ptr<Light> data;
  std::unique_ptr<LightNode> next;
  LightNode* prev;
};

class Shape;
struct ShapeNode{
  std::unique_ptr<Shape> data;
  std::unique_ptr<ShapeNode> next;
  ShapeNode* prev;
};

class Autonoma{
public:
   Camera camera;
   Texture* skybox;
   unsigned int depth;
   std::unique_ptr<ShapeNode> listStart;
   ShapeNode *listEnd;
   std::unique_ptr<LightNode> lightStart;
   LightNode *lightEnd;
   Autonoma(const Camera &c);
   Autonoma(const Camera &c, Texture* tex);
   void addShape(std::unique_ptr<Shape> s);
   void removeShape(ShapeNode* s);
   void addLight(std::unique_ptr<Light> s);
   void removeLight(LightNode* s);
};

void getLight(double* toFill, Autonoma* aut, Vector point, Vector norm, unsigned char r);

#endif
