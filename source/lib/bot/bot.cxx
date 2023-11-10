#include "bot.h"
#include "../tools/tools.h"

#include <exception>

ToDoBot::Bot::Bot() : m_Bot(tools::GetToken()) {
  Init();
  m_Database.Open("database/users.db");
}

void ToDoBot::Bot::Init() {
  auto onStart = [&](TgBot::Message::Ptr message) {
    m_Bot.getApi().sendMessage(message->chat->id, "Hi, " + message->from->username + "!");
    std::string request = "INSERT INTO Users (UserID, IsAdmin) VALUES (" + 
                          std::to_string(message->from->id) + 
                          ", 0)";
    m_Database.ExecuteRequest(request, nullptr);
  };

  auto answerToMessage = [&](Message message) {
    if (message->text.starts_with("/start") or message->text.starts_with("/stop")) { return; }
    m_Bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text, false, message->messageId);
  };

  m_Bot.getEvents().onCommand("start", onStart);
  // m_Bot.getEvents().onCommand("stop", onStop);
  m_Bot.getEvents().onAnyMessage(answerToMessage);
}

void ToDoBot::Bot::Run() {
  int animationCounter = 0;

  try {
    tools::Logger::Message("Bot username:");
    tools::Logger::Message(m_Bot.getApi().getMe()->username.c_str());

    m_Bot.getApi().deleteWebhook();

    LongPoll longPoll(m_Bot);
    
    while (true) {
      // tools::AnimateBot(animationCounter);
      longPoll.start();
    }
  } catch (std::exception& e) {
    tools::Logger::Exception(e.what());
  }
}