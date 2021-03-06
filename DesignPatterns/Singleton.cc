#include <iostream>

class Singleton {
  Singleton(void) {
    std::cout << "Singleton()" << std::endl;
  }

  virtual ~Singleton(void) noexcept {
    std::cout << "~Singleton()" << std::endl;
  }

public:
  static Singleton *getInstance(void) {
    static Singleton local_instance;
    return &local_instance;
  }
};

int main(void) {
  Singleton *singleton = Singleton::getInstance();
  return 0;
}