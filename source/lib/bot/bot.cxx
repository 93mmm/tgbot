#include "bot.h"
#include "../tools/tools.h"
#include "../messages/messages.h"

#include <_types/_uint64_t.h>
#include <exception>

ToDoBot::Bot::Bot() : m_Bot(tools::GetToken()) {
  InitCommands();
  InitCallbacks();
}

void ToDoBot::Bot::InitCallbacks() {
  auto IsMessageCommand = [](const std::string &_message) {
    if (
      _message.starts_with("/start") or
      _message.starts_with("/menu") or 
      _message.starts_with("/language")
      ) { return true; }
    return false;
  };

  auto OnStart = [&](TgBot::Message::Ptr _message) { // TODO: greet, select language
    uint64_t id = _message->from->id;
    tools::SQLite3::Get().AddUser(id);
    tools::SQLite3::Get().GetLanguage(id);

    ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage(tools::SQLite3::Get().GetLanguage(id), LANGUAGE_MESSAGE);

    DeleteMessage(_message->chat->id, _message->messageId);
    m_Bot.getApi().sendMessage(_message->chat->id, answer.text, false, 0, answer.keyboard);
  };

  auto OnMenu = [&](TgBot::Message::Ptr _message) {
    int64_t id = _message->chat->id;
    ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage(tools::SQLite3::Get().GetLanguage(id), MAIN_MESSAGE);

    DeleteMessage(id, _message->messageId);
    m_Bot.getApi().sendMessage(id, answer.text, false, 0, answer.keyboard);
  };

  auto OnLanguageSelect = [&](TgBot::Message::Ptr _message) {
    // TODO: select language
    DeleteMessage(_message->chat->id, _message->messageId);
  };

  auto AnswerToMessage = [&](TgBot::Message::Ptr _message) {
    if (IsMessageCommand(_message->text)) { return; }
    DeleteMessage(_message->chat->id, _message->messageId);
    // m_Bot.getApi().sendMessage(_message->chat->id, "Your message is: " + _message->text, false, _message->messageId);
  };

  auto OnCallbackQuery = [&](TgBot::CallbackQuery::Ptr _query) {
    int64_t chat_id = _query->message->chat->id;
    int32_t msq_id = _query->message->messageId;
    std::string lang = tools::SQLite3::Get().GetLanguage(chat_id);
    
    // TODO: edit order
    if (_query->data.starts_with(Query::Get().GetQuery(Queries::water_menu))) {
      ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage(lang, WATER_MESSAGE);
      m_Bot.getApi().editMessageText(answer.text, chat_id,
                                     msq_id, "", "", false,
                                     answer.keyboard);

    } else if (_query->data.starts_with(Query::Get().GetQuery(Queries::tasks_menu))) {
      ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage(lang, TASKS_MESSAGE);
      m_Bot.getApi().editMessageText(answer.text, chat_id,
                                     msq_id, "", "", false,
                                     answer.keyboard);

    } else if (_query->data.starts_with(Query::Get().GetQuery(Queries::main_menu))) {
      ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage(lang, MAIN_MESSAGE);
      m_Bot.getApi().editMessageText(answer.text, chat_id,
                                     msq_id, "", "", false,
                                     answer.keyboard);
    } else if (_query->data.starts_with(Query::Get().GetQuery(Queries::language_en))) {
      tools::SQLite3::Get().ChangeLanguage(_query->from->id, "en");
      ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage("en", MAIN_MESSAGE);
      m_Bot.getApi().editMessageText(answer.text, chat_id,
                                     msq_id, "", "", false,
                                     answer.keyboard);
    } else if (_query->data.starts_with(Query::Get().GetQuery(Queries::language_ru))) {
      printf("%s\n", _query->data.c_str());
      tools::SQLite3::Get().ChangeLanguage(_query->from->id, "ru");
      ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage("ru", MAIN_MESSAGE);
      m_Bot.getApi().editMessageText(answer.text, chat_id,
                                     msq_id, "", "", false,
                                     answer.keyboard);
    }
  };

  m_Bot.getEvents().onCommand("start", OnStart);
  m_Bot.getEvents().onCommand("menu", OnMenu);
  m_Bot.getEvents().onCommand("language", OnLanguageSelect);
  m_Bot.getEvents().onAnyMessage(AnswerToMessage);
  m_Bot.getEvents().onCallbackQuery(OnCallbackQuery);
}

void ToDoBot::Bot::InitCommands() {
  std::vector<TgBot::BotCommand::Ptr> commands;

  TgBot::BotCommand::Ptr cmd(new TgBot::BotCommand);
  cmd->command = "start";
  cmd->description = "restart bot";
  commands.push_back(cmd);

  cmd = TgBot::BotCommand::Ptr(new TgBot::BotCommand);
  cmd->command = "menu";
  cmd->description = "get menu"; 
  commands.push_back(cmd);

  cmd = TgBot::BotCommand::Ptr(new TgBot::BotCommand);
  cmd->command = "language";
  cmd->description = "change language"; 
  commands.push_back(cmd);

  m_Bot.getApi().setMyCommands(commands);
}

void ToDoBot::Bot::Run() {
  try {
    tools::Logger::DebuggingInfo("Bot username:");
    tools::Logger::DebuggingInfo(m_Bot.getApi().getMe()->username.c_str());

    m_Bot.getApi().deleteWebhook();

    TgBot::TgLongPoll longPoll(m_Bot);
    
    while (true) {
      longPoll.start();
    }
  } catch (std::exception& e) {
    tools::Logger::Error(TGBOT_ERROR, e.what());
  }
}

void ToDoBot::Bot::DeleteMessage(int64_t _chatId, int32_t _msgId) {
  try {
    m_Bot.getApi().deleteMessage(_chatId, _msgId);
  } catch (std::exception& e) {
    tools::Logger::Error(TGBOT_ERROR, e.what());
  }
}