#ifndef SINGLETON_H
#define SINGLETON_H

#include <assert.h>

template <typename T>
class Singleton
{
private:
  static T *m_Singleton;

public:
  Singleton()
  {
    assert(!m_Singleton);
    static long long offset = (long long)(T *)(1LL) - (long long)(Singleton<T> *)(T *)(1LL);
    m_Singleton = (T *)((long long)this + offset);
  }

  ~Singleton()
  {
    assert(m_Singleton);
    m_Singleton = 0;
  }

  static T& GetSingleton()
  {
    assert(m_Singleton);
    return *m_Singleton;
  }

  static T *GetSingletonPtr() { return m_Singleton; }
};

template <typename T> T *Singleton<T>::m_Singleton = nullptr;

#endif