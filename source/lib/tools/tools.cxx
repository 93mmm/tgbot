#include <fstream>

#include "tools.h"

#define ENVIOREMENT_FILE "env.txt"

std::string tools::GetToken() {
  std::ifstream file(ENVIOREMENT_FILE);
  std::string result;

  if (file.is_open()) {
    getline(file, result);
    file.close();
    return result;
  }
  printf("missing required file " ENVIOREMENT_FILE "\n");
  exit(1);
}

void tools::AnimateBot(int &animationCounter) {
  const char *pending = "/-\\|";
  animationCounter = ++animationCounter % 4;
  printf("\x1b[2K\x1b[1F%c\n", pending[animationCounter]);
}


static int ToDoBot::FetchData(void *data, int sizeOfAnswer, char **answer, char **collumnNames) {
  if (data == nullptr) { return 0; }

  DataFromSql *collectedData = (DataFromSql *)data;
  for (int i = 0; i < sizeOfAnswer; i++) {
    collectedData->data[collumnNames[i]].push_back(answer[i]);
  }
  return 0;
}

ToDoBot::Result ToDoBot::SQLite3::Open(std::string fileName) {
  if (sqlite3_open(fileName.c_str(), &m_DB)) {
    printf("Can't open database: %s\n", sqlite3_errmsg(m_DB));
    return Result::failure;
  } 
  printf("Database opened successfully!\n");
  return Result::success;
}

ToDoBot::SQLite3::~SQLite3() {
  sqlite3_close(m_DB);
}

void ToDoBot::SQLite3::ExecuteRequest(const std::string &request, ToDoBot::DataFromSql *data=nullptr) {
  char *errorMessage;

  if (sqlite3_exec(m_DB, request.c_str(), FetchData, data, &errorMessage) == SQLITE_OK) {
    if (data == nullptr) { return; }

    for (const auto &[key, value] : data->data) {
      printf("%s: ", key.c_str());
      for (const auto &i : value) {
        printf("%s, ", i.c_str());
      }
      printf("\n");
    }
  }
}