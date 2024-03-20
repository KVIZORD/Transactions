#ifndef TRANSACTIONS_COMMON_DATA_H_
#define TRANSACTIONS_COMMON_DATA_H_
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
    ss << name << " " << surname << " " << std::to_string(birthday) << " "
       << city << " " << std::to_string(coins);
    return ss.str();
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
#endif  // TRANSACTIONS_COMMON_DATA_H_
