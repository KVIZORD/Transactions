#include "model/common/student.h"

namespace s21 {

bool Student::operator==(const Student& other) const {
  return name == other.name && surname == other.surname &&
         birth_year == other.birth_year && city == other.city &&
         coins == other.coins;
}

std::ostream& operator<<(std::ostream& os, const Student& student) {
  return os << student.name << " " << student.surname << " "
            << student.birth_year << " " << student.city << " "
            << student.coins;
}

std::istream& operator>>(std::istream& in, Student& student) {
  return in >> student.name >> student.surname >> student.birth_year >>
         student.city >> student.coins;
}

}  // namespace s21