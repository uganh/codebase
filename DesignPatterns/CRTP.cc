/*
 * Curiously recurring template pattern
 */

#include <iostream>

template <class T>
class Base {
  friend T;

public:
  void interface(void) {
    static_cast<T *>(this)->impl();
  }

private:
  Base(void) = default;
};

class Derived : public Base<Derived> {
public:
  void impl(void) {
    std::cout << "Derived::impl()" << std::endl;
  }
};

// class BadDerived : public Base<Derived> {};

template <class T>
void doit(Base<T> &ref) {
  /* Static polymorphism */
  ref.interface();
}

int main(void) {
  Derived obj;

  doit(obj);
  
  return 0;
}