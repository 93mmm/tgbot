#include <iostream> // TODO remove
#include <fstream>
#include <assert.h>
#include "tools.h"

#define ENVIOREMENT_FILE "env.txt"

#define RED "\x1b[38;5;196m"
#define YELLOW "\x1b[38;5;118m"
#define GREEN "\x1b[38;5;228m"
#define END "\x1b[0m"

#define ERROR "[WARN]\t"
#define MESSAGE "[MSG]\t"
#define INFO "[INF]\t"

int tools::Logger::s_logLevel = 0;
tools::SQLite3 tools::SQLite3::s_Instance;

static int FetchData(void *_data, int _sizeOfAnswer, char **_answer, char **_collumnNames) {
  if (_data == nullptr) { return 0; }

  tools::SQLite3::DataFromSql *collectedData = (tools::SQLite3::DataFromSql *)_data;
  for (int i = 0; i < _sizeOfAnswer; i++) {
    collectedData->data[_collumnNames[i]].push_back(_answer[i]);
  }
  return 0;
}

void tools::concatenate(const char *_result, const char *_one, const char *_two) {
  assert(strlen(_one) + strlen(_two) < 99);
  char buffer[100];
  strcpy(buffer, _one);
  strcat(buffer, _two);
  _result = buffer;
}

std::string tools::GetToken() {
  std::ifstream file(ENVIOREMENT_FILE);
  std::string result;

  if (not file.is_open()) { 
    tools::Logger::Error(FILE_ERROR, "Missing required file " ENVIOREMENT_FILE);
    exit(1);
  }
  
  getline(file, result);
  file.close();

  if (result == "place your token here") {
    tools::Logger::Error(FILE_ERROR, "Insert your token");
    exit(1);
  }

  return result;
}

void tools::Logger::Error(const char *_errorType, const char *_messagef) {
  if (not (s_logLevel & DISPLAY_ERRORS)) { return; }
  printf(RED ERROR END "(%s)\n\t%s\n", _errorType, _messagef);
}

void tools::Logger::DebuggingInfo(const char *_messagef) {
  if (not (s_logLevel & DISPLAY_DEBUGGING_INFO)) { return; }
  printf(YELLOW INFO END "%s\n", _messagef);
}

void tools::Logger::Message(const char *_messagef) {
  if (not (s_logLevel & DISPLAY_MESSAGES)) { return; }
  printf(GREEN MESSAGE END "%s\n" YELLOW END, _messagef);
}


void tools::SQLite3::ReleaseDatabase() {
  sqlite3_close(m_DB);
}

tools::SQLite3 &tools::SQLite3::Get() { return s_Instance; }

void tools::SQLite3::Init(const char *_fileName) {
  if (sqlite3_open(_fileName, &m_DB)) {
    Logger::Error(DB_ERROR, "Can't open database");
    exit(1);
  }
}

void tools::SQLite3::ExecuteRequest(const char *_request, DataFromSql *_data) {
  char *errorMessage;

  if (sqlite3_exec(m_DB, _request, FetchData, _data, &errorMessage) != SQLITE_OK) {
    Logger::Error(DB_ERROR, "error while executing request:");
    Logger::Message(_request);
    Logger::Message(std::string(errorMessage).c_str());
    return;
  }
  if (_data == nullptr) { return; }

  for (const auto &[key, value] : _data->data) {
    printf("%s: ", key.c_str());
    for (const auto &i : value) { printf("%s, ", i.c_str()); }

    printf("\n");
  }
}

void tools::SQLite3::AddUser(int64_t _id) { // TODO: rebase on c-style strings
  std::string request = "INSERT INTO Users (UserID) VALUES (" + 
                        std::to_string(_id) + ")";
  ExecuteRequest(request.c_str(), nullptr);
}

void tools::SQLite3::ChangeLanguage(int64_t _id, const char *_lang) {
  std::string request = "UPDATE Users SET Language = '";
  request += _lang;
  request += "' WHERE UserID = " + std::to_string(_id);

  ExecuteRequest(request.c_str(), nullptr);
}

std::string tools::SQLite3::GetLanguage(int64_t _id) {
  DataFromSql data;
  std::string request = "SELECT Language FROM Users WHERE UserID = " + std::to_string(_id);

  ExecuteRequest(request.c_str(), &data);
  return data.data["Language"][0];
}
