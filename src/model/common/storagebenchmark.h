#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_STORAGEBENCHMARK_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_STORAGEBENCHMARK_H_

#include "controller/controller.h"
#include "model/common/student.h"

namespace s21 {

class StorageBenchmark {
 public:
  static void FillStorage(Controller<std::string, Student>& controller,
                          size_t count);
  static double MeasureGet(Controller<std::string, Student>& storage,
                           size_t repeats);
  static double MeasureShowAll(Controller<std::string, Student>& storage,
                               size_t repeats);
  static double MeasureFind(Controller<std::string, Student>& storage,
                            size_t repeats);
  static double MeasureSet(Controller<std::string, Student>& storage,
                           size_t repeats);
  static double MeasureDel(Controller<std::string, Student>& storage,
                           size_t repeats);

 private:
  static std::string GenerateRandomString(size_t length);
  static Student GenerateStudent();
  static std::string GenerateKey(size_t length);
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_STORAGEBENCHMARK_H_