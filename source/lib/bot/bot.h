#pragma once

#include <tgbot/tgbot.h>
#include "../tools/tools.h"

namespace ToDoBot {
  class Bot {
    public:
      Bot(std::string _token);
      Bot() = delete;
      void Run();

    private:
      void InitCallbacks();
      void InitCommands();

      void DeleteMessage(int64_t _chatId, int32_t _msgId);
      void SendMessage(int64_t _chatId, std::string _text, TgBot::InlineKeyboardMarkup::Ptr _keyboard);
      void EditMessage(int64_t _chatId, int32_t _msgId, const std::string &_text, TgBot::InlineKeyboardMarkup::Ptr _keyboard);

      TgBot::Bot m_Bot;
  };
}