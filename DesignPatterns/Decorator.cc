#include <iostream>
#include <string>

/*
 * Component
 * */
class View {
public:
  virtual void draw(void) const = 0;
};

/*
 * A concrete component
 * */
class TextView : public View {
  std::string text;

public:
  TextView(const std::string &text) : text(text) {}

  void draw(void) const override {
    std::cout << "TextView(\"" << text << "\")" << std::endl;
  }
};

/*
 * Decorator
 * */
class Decorator : public View {
  View &view;

public:
  Decorator(View &view) : view(view) {}

  void draw(void) const override {
    view.draw();
  }
};

/*
 * A concrete decorator
 * */
class BorderDecorator : public Decorator {
  unsigned width;

public:
  BorderDecorator(View &view, unsigned width) : Decorator(view), width(width) {}

  void draw(void) const override {
    Decorator::draw();
    std::cout << "Border width: " << width << std::endl;
  }
};

/*
 * Another concrete decorator
 * */
class ScrollDecorator : public Decorator {
  unsigned position;

public:
  ScrollDecorator(View &view) : Decorator(view), position(0) {}

  void draw(void) const override {
    Decorator::draw();
    std::cout << "With scroll bar" << std::endl;
  }

  void scrollTo(unsigned position) {
    this->position = position;
  }
};

int main(void) {
  TextView view("Hello");
  ScrollDecorator scroll(view);
  BorderDecorator border(scroll, 1);

  View &viewRef = border;
  viewRef.draw();

  return 0;
}