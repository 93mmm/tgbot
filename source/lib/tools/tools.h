#pragma once

#include <sqlite3.h>
#include <unordered_map>
#include <string>
#include <vector>

#define DISPLAY_MESSAGES 1
#define DISPLAY_DEBUGGING_INFO 2
#define DISPLAY_ERRORS 4

#define FILE_ERROR "File error"
#define DB_ERROR "Databse error"
#define TGBOT_ERROR "Telegram bot error"

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
    SQLite3(const SQLite3 &) = delete;
    ~SQLite3();

    static SQLite3 &Get();
    void Init(const char *_fileName);


    void ExecuteRequest(const char *_request, DataFromSql *_data);
  private:
    SQLite3() {}
    static SQLite3 s_Instance;
    sqlite3 *m_DB = nullptr;
  };
}