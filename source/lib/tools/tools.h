#pragma once

#include <sqlite3.h>
#include <unordered_map>
#include <string>
#include <vector>

#define DISPLAY_MESSAGES 1
#define DISPLAY_DEBUGGING_INFO 2
#define DISPLAY_ERRORS 4

#define FILE_ERROR "file error"
#define DB_ERROR "databse error"
#define TGBOT_ERROR "telegram bot error"

namespace tools {
  std::string GetToken();

  class Logger { 
  public:
    static void SetLevel(const int &_level) { s_logLevel = _level; }
    
    static void Error(const char *_error_type,
                      const char *_messagef);
    static void Message(const char *_messagef);
    static void DebuggingInfo(const char *_messagef);

  private:
    static int s_logLevel;
  };
}

namespace tools {
  static int FetchData(void *data, int sizeOfAnswer, char **answer, char **collumnNames);
  
  struct DataFromSql {
    std::unordered_map<std::string, std::vector<std::string> > data;
  };

  class SQLite3 {
  public:
    int Open(const char *_fileName);
    ~SQLite3();
    void ExecuteRequest(const char *request, DataFromSql *data);

  private:
    sqlite3 *m_DB = nullptr;
  };
}
