#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template <class T>
class Singleton {
 public:
  static T* Instance() {
    static T inst;
    return &inst;
  }

  Singleton() = delete;
  ~Singleton() = delete;
  Singleton(const Singleton &) = delete;
  Singleton(Singleton &&) = delete;
  Singleton& operator=(const Singleton &) = delete;
  Singleton& operator=(Singleton &&) = delete;
};

#endif