#include <iostream>
#include "lib/bot/bot.h"
#include "lib/messages/messages.h"

int main() {
  signal(SIGINT, [](int s) {
    tools::SQLite3::Get().ReleaseDatabase();
    exit(0);
  });

  ToDoBot::Languages::Get().Init();
  ToDoBot::Query::Get().Init();
  tools::SQLite3::Get().Init("database/users.db");
  tools::Logger::SetLevel(DISPLAY_MESSAGES | DISPLAY_DEBUGGING_INFO | DISPLAY_ERRORS);

  ToDoBot::Bot bot;
  bot.Run();

  return 0;
}
