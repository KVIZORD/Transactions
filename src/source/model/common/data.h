#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_DATA_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_DATA_H_

#include <iomanip>
#include <istream>
#include <sstream>
#include <string>

namespace s21 {
struct Student {
 public:
  std::string name;
  std::string surname;
  int birthday{0};
  std::string city;
  int coins{0};
  Student() = default;
  Student(std::string name, std::string surname, int birthday, std::string city,
          int coins) {
    this->name = name;
    this->surname = surname;
    this->birthday = birthday;
    this->city = city;
    this->coins = coins;
  }
  std::string ToString() {
    std::stringstream ss;
    ss << std::left << std::setw(15) << name << std::left << std::setw(15)
       << surname << std::left << std::setw(10) << std::to_string(birthday)
       << std::left << std::setw(15) << city << std::left << std::setw(8)
       << std::to_string(coins);
    return ss.str();
  }

  bool operator==(const Student& other) const {
    return name == other.name && surname == other.surname &&
           birthday == other.birthday && city == other.city &&
           coins == other.coins;
  }

  friend std::ostream& operator<<(std::ostream& os, const Student& student) {
    return os << student.name << " " << student.surname << " "
              << student.birthday << " " << student.city << " "
              << student.coins;
  }

  friend std::istream& operator>>(std::istream& in, Student& student) {
    in >> student.name >> student.surname >> student.birthday >> student.city >>
        student.coins;

    return in;
  }

  bool IsEmpty() const {
    if (name == "" && surname == "" && birthday == 0 && city == "" &&
        coins == 0)
      return true;
    return false;
  }
};

class Data {
 public:
  Data() = default;
  Data(std::string key, Student value, int validity = 0)
      : key_(key), value_(value), validity_(validity) {}

  std::string GetKey() { return key_; }
  Student GetValue() { return value_; }
  int GetValidity() { return validity_; }
  void SetKey(std::string key) { key_ = key; }
  void SetValue(Student value) { value_ = value; }
  void SetValidity(int validity) { validity_ = validity; }

 private:
  std::string key_;
  Student value_;
  int validity_{0};
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_DATA_H_