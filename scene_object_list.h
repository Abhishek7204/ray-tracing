#ifndef SCENE_OBJECT_LIST_H
#define SCENE_OBJECT_LIST_H

#include "scene_object.h"
#include <memory>
#include <vector>

class sceneObjectList : public sceneObject {
  vector<shared_ptr<sceneObject>> objectList;
  aabb bbox;

public:
  void clear() { objectList.clear(); }

  void add(shared_ptr<sceneObject> object);

  virtual bool isHit(const ray &r, interval ray_t,
                     hitRecord &record) const override;

  aabb boundingBox() const override { return bbox; }
};
#endif // !SCENE_OBJECT_LIST_H
