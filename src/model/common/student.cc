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

bool StudentComparator::operator()(const Student& student_1,
                                   const Student& student_2) const {
  bool name_equal =
      (student_1.name == student_2.name || student_2.name == kStrIgnored);
  bool surname_equal = (student_1.surname == student_2.surname ||
                        student_2.surname == kStrIgnored);
  bool birhday_equal = (student_1.birth_year == student_2.birth_year ||
                        student_2.birth_year == kIntIgnored);
  bool city_equal =
      (student_1.city == student_2.city || student_2.city == kStrIgnored);
  bool coins_equal =
      (student_1.coins == student_2.coins || student_2.coins == kIntIgnored);

  return name_equal && surname_equal && birhday_equal && city_equal &&
         coins_equal;
}

}  // namespace s21