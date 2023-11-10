#include <fstream>

#include "tools.h"

#define ENVIOREMENT_FILE "env.txt"

int tools::Logger::s_logLevel = 0;

std::string tools::GetToken() {
  std::ifstream file(ENVIOREMENT_FILE);
  std::string result;

  if (file.is_open()) {
    getline(file, result);
    file.close();
    return result;
  }
  tools::Logger::Exception("missing required file " ENVIOREMENT_FILE "\n");
  exit(1);
}

void tools::AnimateBot(int &animationCounter) {
  const char *pending = "/-\\|";
  animationCounter = ++animationCounter % 4;
  printf("\x1b[2K\x1b[1F%c\n", pending[animationCounter]);
}

void tools::Logger::Message(const char *_messagef) {
  if (not (s_logLevel & DISPLAY_MESSAGES)) { return; }
  printf("\x1b[38;5;118m[MSG]\t\x1b[0m%s\n", _messagef);
}

void tools::Logger::DebuggingInfo(const char *_messagef) {
  if (not (s_logLevel & DISPLAY_DEBUGGING_INFO)) { return; }
  printf("\x1b[38;5;228m[INF]\t\x1b[0m%s\n", _messagef);
}

void tools::Logger::Exception(const char *_messagef) {
  if (not (s_logLevel & DISPLAY_ERRORS)) { return; }
  printf("\x1b[38;5;196m[ERR]\t\x1b[0m%s\n", _messagef);
}

int tools::FetchData(void *data, int sizeOfAnswer, char **answer, char **collumnNames) {
  if (data == nullptr) { return 0; }

  DataFromSql *collectedData = (DataFromSql *)data;
  for (int i = 0; i < sizeOfAnswer; i++) {
    collectedData->data[collumnNames[i]].push_back(answer[i]);
  }
  return 0;
}

tools::Result tools::SQLite3::Open(std::string fileName) {
  if (sqlite3_open(fileName.c_str(), &m_DB)) {
    printf("Can't open database: %s\n", sqlite3_errmsg(m_DB));
    return Result::failure;
  } 
  printf("Database opened successfully!\n");
  return Result::success;
}

tools::SQLite3::~SQLite3() {
  sqlite3_close(m_DB);
}

void tools::SQLite3::ExecuteRequest(const std::string &request, tools::DataFromSql *data=nullptr) {
  char *errorMessage;

  if (
    sqlite3_exec(m_DB, request.c_str(), FetchData, data, &errorMessage) != SQLITE_OK
    or
    data == nullptr
    ) { return; }

  for (const auto &[key, value] : data->data) {
    printf("%s: ", key.c_str());
    for (const auto &i : value) { printf("%s, ", i.c_str()); }

    printf("\n");
  }
}