#include <iostream>

struct InstructionVisitor;

struct Instruction {
  virtual void accept(InstructionVisitor &);
};

struct Load : public Instruction {
  void accept(InstructionVisitor &) override;
};

struct Move : public Instruction {
  void accept(InstructionVisitor &) override;
};

struct InstructionVisitor {
  void visitInstruction(Instruction &instr) {
    std::cout << "Instruction" << std::endl;
  }

  void visitLoad(Load &instr) {
    std::cout << "Load" << std::endl;
  }

  void visitMove(Move &instr) {
    std::cout << "Move" << std::endl;
  }
};

void Instruction::accept(InstructionVisitor &iv)  {
  iv.visitInstruction(*this);
}

void Load::accept(InstructionVisitor &iv)  {
  iv.visitLoad(*this);
  // Call base class function
  Instruction::accept(iv);
}

void Move::accept(InstructionVisitor &iv)  {
  iv.visitMove(*this);
  Instruction::accept(iv);
}

int main(void) {
  Load load;
  Move move;

  InstructionVisitor iv;

  static_cast<Instruction &>(load).accept(iv);
  static_cast<Instruction &>(move).accept(iv);

  return 0;
}