#include "bot.h"

#include <exception>

ToDoBot::Bot::Bot() : bot(tools::GetToken()) {
  Init();
  database.Open("database/test.db");
}

void ToDoBot::Bot::Init() {
  auto onStart = [&](TgBot::Message::Ptr message) {
    bot.getApi().sendMessage(message->chat->id, "Hi, " + message->from->username + "!");
    std::string request = "INSERT INTO Users (UserID, IsAdmin) VALUES (" + 
                          std::to_string(message->from->id) + 
                          ", 0)";
    database.ExecuteRequest(request, nullptr);
  }; 

  bot.getEvents().onCommand("start", onStart);
  bot.getEvents().onCommand("stop", onStart);

  auto answerToMessage = [&](Message message) {
    bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text, false, message->messageId);
  };

  bot.getEvents().onAnyMessage(answerToMessage);
}

void ToDoBot::Bot::Run() {
  int animationCounter = 0;

  try {
    printf("Bot username: %s\n-\n", bot.getApi().getMe()->username.c_str());
    bot.getApi().deleteWebhook();

    LongPoll longPoll(bot);
    
    while (true) {
      tools::AnimateBot(animationCounter);
      longPoll.start();
    }
  } catch (std::exception& e) {
    printf("error: %s\n", e.what());
  }
}