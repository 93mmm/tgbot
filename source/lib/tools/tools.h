#pragma once

#include <sqlite3.h>
#include <unordered_map>
#include <string>
#include <vector>

#define DISPLAY_MESSAGES 1
#define DISPLAY_DEBUGGING_INFO 2
#define DISPLAY_ERRORS 4

namespace tools {
  std::string GetToken();
  void AnimateBot(int &animationCounter);

  class Logger { 
  public:
    static void SetLevel(const int &_level) { s_logLevel = _level; }
    
    static void Message(const char *_messagef);
    static void DebuggingInfo(const char *_messagef);
    static void Exception(const char *_messagef);

  private:
    static int s_logLevel;
  };
}

// TODO: refactor
namespace tools {
  static int FetchData(void *data, int sizeOfAnswer, char **answer, char **collumnNames);
  
  struct DataFromSql {
    std::unordered_map<std::string, std::vector<std::string> > data;
  };

  enum Result {
    success,
    failure
  };

  class SQLite3 {

  public:
    Result Open(std::string fileName);
    ~SQLite3();
    void ExecuteRequest(const std::string &request, DataFromSql *data);

  private:
    sqlite3 *m_DB = nullptr;
  };
}
