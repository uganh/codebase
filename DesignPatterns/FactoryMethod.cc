#include <iostream>

/*
 * A C++ implementation using template
 * */

/*
 * Product
 * */
class Product {};

/*
 * Creator
 * */
class Creator {
public:
  virtual Product *Create(void) = 0;
};

template <class TheProduct>
class StandardCreator : public Creator {
  virtual Product *Create(void) {
    return new TheProduct();
  }
};

/*
 * Another example
 * */

/*
 * Product
 * */
class Manipulator {
public:
  virtual ~Manipulator(void) noexcept = default;
  virtual void DownClick(void) = 0;
  virtual void Drag(void) = 0;
  virtual void UpClick(void) = 0;
};

class LineManipulator : public Manipulator {
  virtual void DownClick(void) override {
    std::cout << "Down click on Line" << std::endl;
  }

  virtual void Drag(void) override {
    std::cout << "Drag on Line" << std::endl;
  }

  virtual void UpClick(void) override {
    std::cout << "Up click on Line" << std::endl;
  }
};

class TextManipulator : public Manipulator {
  virtual void DownClick(void) override {
    std::cout << "Down click on Text" << std::endl;
  }

  virtual void Drag(void) override {
    std::cout << "Drag on Text" << std::endl;
  }
  
  virtual void UpClick(void) override {
    std::cout << "Up click on Text" << std::endl;
  }
};

class Figure {
public:
  virtual Manipulator *CreateManipulator(void) = 0;
};

class LineFigure : public Figure {
public:
  virtual Manipulator *CreateManipulator(void) override {
    return new LineManipulator();
  }
};

class TextFigure : public Figure {
public:
  virtual Manipulator *CreateManipulator(void) override {
    return new TextManipulator();
  }
};

void Client(Figure &Fig) {
  Manipulator *M = Fig.CreateManipulator();
  M->DownClick();
  M->Drag();
  M->UpClick();
  delete M;
}

int main(void) {
  LineFigure Line;
  TextFigure Text;

  Client(Line);
  Client(Text);

  return 0;
}