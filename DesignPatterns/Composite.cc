#include <iostream>
#include <string>
#include <vector>

/*
 * Component: It can be used as a composite or leaf
 * */
class Employee {
  friend std::ostream &operator<<(std::ostream &, const Employee &);

  std::string name;
  std::string position;
  std::vector<Employee *> subordinates;

public:
  Employee(const std::string &name, const std::string &position) :
    name(name), position(position) {}

  void add(Employee *employee) {
    subordinates.push_back(employee);
  }

  void remove(Employee *employee) {
    auto iter = std::find(subordinates.begin(), subordinates.end(), employee);
    if (iter != subordinates.end()) {
      subordinates.erase(iter);
    }
  }

  const std::vector<Employee *> getSubordinates(void) const {
    return subordinates;
  }
};

std::ostream &operator<<(std::ostream &stream, const Employee &employee) {
  stream << "Employee: " << employee.name << ", " << employee.position;
  if (!employee.subordinates.empty()) {
    stream << " {" << std::endl;
    for (Employee *subordinate : employee.subordinates) {
      stream << *subordinate << std::endl;
    }
    stream << "}";
  }
  return stream;
}

int main(void) {
  Employee John("John", "CEO");

  Employee Robert("Robert", "Head Sales");
  
  Employee Michel("Michel", "Head Marketing");

  John.add(&Robert);
  John.add(&Michel);

  Employee Richard("Richard", "Sales");
  Employee Rob("Rob", "Sales");

  Robert.add(&Richard);
  Robert.add(&Rob);

  Employee Laura("Laura", "Marketing");
  Employee Bob("Bob", "Marketing");

  Michel.add(&Laura);
  Michel.add(&Bob);

  std::cout << John << std::endl;

  return 0;
}