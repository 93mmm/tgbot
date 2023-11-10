#include <iostream>
#include "lib/bot/bot.h"

int main() {
  ToDoBot::Bot bot;
  tools::Logger::SetLevel(DISPLAY_MESSAGES | DISPLAY_DEBUGGING_INFO | DISPLAY_ERRORS);
  bot.Run();
  return 0;
}
