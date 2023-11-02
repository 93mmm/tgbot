#include "bot.h"

#include <exception>

ToDoBot::Bot::Bot() : bot(tools::GetToken()) { Init(); }

void ToDoBot::Bot::Init() {
  auto answerToMessage = [&](Message message) {
    bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text, false, message->messageId);
  };
  bot.getEvents().onAnyMessage(answerToMessage);
}

void ToDoBot::Bot::Run() {
  try {
    printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
    bot.getApi().deleteWebhook();

    LongPoll longPoll(bot);
    while (true) {
      printf("Long poll started\n");
      longPoll.start();
    }
  } catch (std::exception& e) {
    printf("error: %s\n", e.what());
  }

}