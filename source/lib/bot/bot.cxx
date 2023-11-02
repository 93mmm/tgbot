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