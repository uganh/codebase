#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

class Object {
public:
  virtual void say(void) = 0;
  virtual ~Object(void) noexcept = default;
};

template <typename... Args> class ObjectFactory {
  static auto &getRegistry(void) {
    static std::unordered_map<std::string, std::function<Object *(Args && ...)>>
        registry;
    return registry;
  }

public:
  static bool Register(const std::string &name,
                       std::function<Object *(Args &&...)> ctor) {
    return ctor && getRegistry().insert({name, ctor}).second;
  }

  static Object *Create(const std::string &name, Args &&...args) {
    auto iter = getRegistry().find(name);
    if (iter != getRegistry().end()) {
      return iter->second(std::forward<Args>(args)...);
    }
    return nullptr;
  }
};

/*
 * For 'Args', const and reference need to be removed
 */
template <typename Ty, typename... Args> class DynamicCreator {
  struct Register {
    Register(void) {
      std::string clsname(typeid(Ty).name());
      ObjectFactory<Args...>::Register(clsname, Creator);
      std::cout << "Register: '" << clsname << "'" << std::endl;
    }

    /*
     * Enable 'R' to be forcibly instantiated
     */
    void dummy(void) const {}
  };

  static Register R;

  static Ty *Creator(Args &&...args) {
    try {
      return new Ty(std::forward<Args>(args)...);
    } catch (std::bad_alloc &) {
      return nullptr;
    }
  }

protected:
  DynamicCreator(void) { R.dummy(); }
};

template <typename Ty, typename... Args>
typename DynamicCreator<Ty, Args...>::Register DynamicCreator<Ty, Args...>::R;

class Foo : public Object, DynamicCreator<Foo> {
public:
  Foo(void) {}

  virtual void say(void) override { std::cout << "Foo.say()" << std::endl; }
};

class Bar : public Object, DynamicCreator<Bar, std::string, int> {
public:
  Bar(const std::string &name, int id) {}

  virtual void say(void) override { std::cout << "Bar.say()" << std::endl; }
};

template <typename... Args>
Object *CreateObject(const std::string &clsname, Args &&...args) {
  return ObjectFactory<Args...>::Create(clsname, std::forward<Args>(args)...);
}

int main(void) {
  Object *foo = CreateObject("class Foo");
  foo->say();
  delete foo;

  Object *bar = CreateObject("class Bar", std::string("Hello"), 5);
  bar->say();
  delete bar;

  return 0;
}