#include <fstream>
#include <string>

#include "tools.h"

#define ENVIOREMENT_FILE "env.txt"

std::string tools::GetToken() {
  std::ifstream file(ENVIOREMENT_FILE);
  std::string result;

  if (file.is_open()) {
    getline(file, result);
    file.close();
  } else {
    printf("missing required file " ENVIOREMENT_FILE "\n");
    file.close();
    exit(1);
  }

  return result;
}

void tools::AnimateBot(int &animationCounter) {
  const char *pending = "/-\\|";
  animationCounter = ++animationCounter % 4;
  printf("\x1b[2K\x1b[1F%c\n", pending[animationCounter]);
}