#include "view/storagebenchmarkmenu.h"

int main() {
  s21::StorageBenchmarkMenu<std::string, s21::Student> menu;
  menu.Start();

  return 0;
}
