#include <iostream>
#include "lib/bot/bot.h"
#include "lib/messages/messages.h"

#define ENVIOREMENT_FILE "env.txt"
#define JSON_FILE "json/languages.json"
#define DATABASE_FILE "database/users.db"

int main() {
  signal(SIGINT, [](int s) {
    tools::SQLite3::Get().ReleaseDatabase();
    exit(0);
  });

  ToDoBot::Languages::Get().Init("json/languages.json");
  ToDoBot::Query::Get().Init("json/languages.json");
  tools::SQLite3::Get().Init("database/users.db");
  tools::Logger::SetLevel(DISPLAY_MESSAGES | DISPLAY_DEBUGGING_INFO | DISPLAY_ERRORS);

  ToDoBot::Bot bot(tools::GetToken(ENVIOREMENT_FILE));
  bot.Run();

  return 0;
}
