#pragma once

#include <sqlite3.h>
#include <unordered_map>
#include <string>
#include <vector>

namespace tools {

  std::string GetToken();
  void AnimateBot(int &animationCounter);

}

namespace ToDoBot {
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
