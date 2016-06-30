#ifndef COMPONENT_H
#define COMPONENT_H

// Cool idea to use system-specific components to hold registration information,
// would be cooler to auto-generate registration functions for components,
// but that can be looked at later.

class BaseComponent
{
  public:
    virtual void Register() {};
};

template <typename T>
class Component : public BaseComponent
{
};

template <typename T, typename U>
class SystemComponent : public Component<T>
{
  public:
    virtual void Register() final
    {
      U::GetSingletonPtr()->RegisterComponent(T::name, this);
    }
};


#endif