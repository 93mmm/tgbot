#pragma once

#include <tgbot/tgbot.h>
#include "../tools/tools.h"

namespace ToDoBot {
  class Bot {
    public:
      Bot();
      void Run();

    private:
      void InitCallbacks();
      void InitCommands();

      void DeleteMessage(int64_t _chatId, int32_t _msgId);
      void SendMessage(); // TODO

      TgBot::Bot m_Bot;
  };
}