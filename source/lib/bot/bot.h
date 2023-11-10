#pragma once

#include <tgbot/tgbot.h>
#include "../tools/tools.h"

namespace ToDoBot {
  using Message = TgBot::Message::Ptr;
  using TgBotLib = TgBot::Bot;
  using LongPoll = TgBot::TgLongPoll;

  class Bot {
    public:
      Bot();
      void Run();

    private:
      void Init();
      TgBot::Bot m_Bot;
      tools::SQLite3 m_Database;
  };
}