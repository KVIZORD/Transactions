#ifndef TRANSACTIONS_SOURCE_MODEL_PARSER_PARSER_H_
#define TRANSACTIONS_SOURCE_MODEL_PARSER_PARSER_H_

#include <istream>
#include <limits>
#include <string>

namespace s21 {

class Parser {
 public:
  inline static const std::string kErrorParse = "Failed to read";

  template <typename T>
  T ParseValue(std::istream& stream, const std::string& arg_name) {
    T value;
    stream >> value;
    if (stream.fail()) {
      stream.clear();
      stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      throw std::invalid_argument(kErrorParse + " \'" + arg_name + "\'");
    }

    return value;
  }

  template <typename T>
  std::pair<std::string, T> ParseOptionalArgument(std::istream& stream,
                                                  const std::string& arg_name) {
    if (stream.eof()) {
      return std::make_pair("", T());
    }

    std::string parsed_name = ParseValue<std::string>(stream, arg_name);
    std::transform(parsed_name.begin(), parsed_name.end(), parsed_name.begin(),
                   ::tolower);
    if (parsed_name != arg_name) {
      throw std::invalid_argument("Not accepted arg: " + parsed_name);
    }

    T value = ParseValue<T>(stream, "positional argument");

    return std::make_pair(parsed_name, value);
  }
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_PARSER_PARSER_H_
