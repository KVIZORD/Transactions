#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_STUDENT_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_STUDENT_H_

#include <istream>
#include <limits>
#include <ostream>
#include <string>

namespace s21 {
struct Student {
  std::string name;
  std::string surname;
  int birth_year{0};
  std::string city;
  int coins{0};

  bool operator==(const Student& other) const;

  friend std::ostream& operator<<(std::ostream& os, const Student& student);

  friend std::istream& operator>>(std::istream& in, Student& student);
};

struct StudentComparator {
  const std::string kStrIgnored = "-";
  const int kIntIgnored = std::numeric_limits<int>::min();

  bool operator()(const Student& student_1, const Student& student_2) const;
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_STUDENT_H_