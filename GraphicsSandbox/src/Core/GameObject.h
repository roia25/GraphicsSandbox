#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <unordered_map>

#include "Component.h"

class Transform : public Component<Transform>
{
public:
  glm::vec3 position;
  glm::vec2 scale;
  float rotation;
};

class GameObject
{
  private:
    std::unordered_map<std::string, BaseComponent *> componentMap;
  
  public:
    template <typename T>
    T *AddComponentInternal(T *component, std::string& componentName);
    
    template <typename T>
    T *GetComponentInternal(std::string& componentName);

    Transform *transform;

};

template <typename T>
T *GameObject::AddComponentInternal(T *component, std::string& componentName)
{
  componentMap[componentName] = component;
  return component;
}

template <>
Transform *GameObject::AddComponentInternal<Transform>(Transform *component, std::string& componentName)
{
  transform = component;
  componentMap[componentName] = component;
  return component;
}

template <typename T>
T *GameObject::GetComponentInternal(std::string& componentName)
{
  auto it = componentMap.find(componentName);

  if (it == componentMap.end())
    return nullptr;
  else
    return static_cast<T *>(it->second);
}

template <>
Transform *GameObject::GetComponentInternal<Transform>(std::string& componentName)
{
  (void)(componentName);
  return transform;
}

#define AddComponent(TYPE, ...) AddComponentInternal(new TYPE(__VA_ARGS__), std::string(#TYPE))
#define GetComponent(TYPE) GetComponentInternal<TYPE>(std::string(#TYPE))

#endif
