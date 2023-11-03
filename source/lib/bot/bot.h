#pragma once
#ifndef TODO_BOT_H
#define TODO_BOT_H

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
      TgBot::Bot bot;
      ToDoBot::SQLite3 database;
  };
}

#endif // TODO_BOT_H