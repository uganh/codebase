#include <cassert>

class Base {
public:
  virtual ~Base(void) noexcept = default;
  virtual void Do(void) {}
};

class Derived : public Base {
public:
  virtual void Do(void) override {}
};

int main(void) {
  long i = 10;
  void *vp = &i;
  const char *ccp;

  /* Any well-defined type conversion, other than those involving low-level
   * const, can be requested using a static_cast */
  double slope = static_cast<double>(5) / i;
  long *ip = static_cast<long *>(vp);
  const long *cip = static_cast<const long *>(ip);
  const long &cir = static_cast<const long &>(i);
  // Grammatically correct but not make sense
  double *dp = static_cast<double *>(vp);
  // Type conversion error
  // double *dp2  = static_cast<double *>(ip);

  Base b, *bptr = &b;
  Derived d, *dptr = &d;
  Base *ptr = dptr;

  static_cast<Base>(d);

  /* The static_cast can also cast through inheritance hierarchies. When casting
   * downwards it can be used as long as it doesn't cast through virtual
   * inheritance. It does not do checking, however, and it is undefined behavior
   * to static_cast down a hierarchy to a type that isn't actually the type of
   * the object.
   */

  static_cast<Derived *>(ptr);
  // undefined behavior
  // static_cast<Derived *>(bptr);

  /* A const_cast changes only a low-level const in its operand */
  char *cp = const_cast<char *>(ccp);

  /*
   * The dynamic_cast is exclusively used for handling polymorphism. You can use
   * it for more than just casting downwards - you can cast sideways or even up
   * another chain. The dynamic_cast will seek out the desired object and return
   * it if possible. If it can't, it will return nullptr in the case of a
   * pointer, or throw std::bad_cast in the case of a reference.
   */

  assert(dynamic_cast<Base *>(dptr));
  assert(dynamic_cast<Derived *>(ptr));
  assert(dynamic_cast<Derived *>(bptr) == nullptr);

  /* A reinterpret_cast generally performs a low-level reinterpretation of the
   * bit pattern of its operands */
  double *dp2 = reinterpret_cast<double *>(ip);

  /* C-style cast and function-style cast are casts using `(type)object` or
   * `type(object)`, respectively, and are functionally equivalent. The are
   * defined as the first of the following which succeeds:
   *
   * const_cast
   * static_cast
   * static_cast + const_cast
   * reinterpret_cast
   * reinterpret_cast + const_cast 
   * */

  return 0;
}