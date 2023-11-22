#include "bot.h"
#include "../tools/tools.h"
#include "../messages/messages.h"

#include <cstdio>
#include <exception>

ToDoBot::Bot::Bot(std::string _token) : m_Bot(_token) {
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
    int64_t id = _message->from->id;
    tools::SQLite3::Get().AddUser(id);

    ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage(tools::SQLite3::Get().GetLanguage(id), LANGUAGE_MESSAGE);

    DeleteMessage(_message->chat->id, _message->messageId);
    SendMessage(_message->chat->id, answer.text, answer.keyboard);
  };

  auto OnMenu = [&](TgBot::Message::Ptr _message) {
    int64_t id = _message->chat->id;
    ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage(tools::SQLite3::Get().GetLanguage(id), MAIN_MESSAGE);

    DeleteMessage(id, _message->messageId);
    SendMessage(id, answer.text, answer.keyboard);
  };

  auto OnLanguageSelect = [&](TgBot::Message::Ptr _message) {
    int64_t _chatId = _message->chat->id;
    ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage(tools::SQLite3::Get().GetLanguage(_chatId), LANGUAGE_MESSAGE);
    DeleteMessage(_chatId, _message->messageId);
    SendMessage(_chatId, answer.text, answer.keyboard);
  };

  auto AnswerToMessage = [&](TgBot::Message::Ptr _message) {
    if (IsMessageCommand(_message->text)) { return; }
  
    DeleteMessage(_message->chat->id, _message->messageId);
  };

  auto OnCallbackQuery = [&](TgBot::CallbackQuery::Ptr _query) {
    int64_t chatId = _query->message->chat->id;
    int64_t fromId = _query->from->id;
    int32_t msgId = _query->message->messageId;
    std::string lang = tools::SQLite3::Get().GetLanguage(chatId);
    
    Queries query = Query::Get().GetQueryId(_query->data);

    switch (query) {
      case Queries::main_menu: {
        ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage(lang, MAIN_MESSAGE);
        EditMessage(chatId, msgId, answer.text, answer.keyboard);
        break;
      }
      case Queries::tasks_menu: {
        ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage(lang, TASKS_MESSAGE);
        EditMessage(chatId, msgId, answer.text, answer.keyboard);
        break;
      }
      case Queries::water_menu: {
        ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage(lang, WATER_MESSAGE);
        EditMessage(chatId, msgId, answer.text, answer.keyboard);
        break;
      }
      case Queries::contact_with_creator: { break; }
      case Queries::add_100:  { break; }
      case Queries::add_200: { break; }
      case Queries::add_300: { break; }
      case Queries::add_400: { break; }
      case Queries::add_other_volume: { break; }
      case Queries::add_task: { break; }
      case Queries::send_current_tasks: { break; }
      case Queries::send_completed_tasks: { break; }
      case Queries::language_ru: {
        tools::SQLite3::Get().ChangeLanguage(fromId, "ru");
        ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage("ru", MAIN_MESSAGE);
        EditMessage(chatId, msgId, answer.text, answer.keyboard);
        break;
      }
      case Queries::language_en: {
        tools::SQLite3::Get().ChangeLanguage(fromId, "en");
        ToDoBot::Message answer = ToDoBot::Languages::Get().GetMessage("en", MAIN_MESSAGE);
        EditMessage(chatId, msgId, answer.text, answer.keyboard);
        break;
      }
      case Queries::__MaxNumber: {
        tools::Logger::Error(QUERY_ERROR, "Unhandled Query");
        break;
      }
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

void ToDoBot::Bot::SendMessage(int64_t _chatId, std::string _text, TgBot::InlineKeyboardMarkup::Ptr _keyboard) {
  try {
    m_Bot.getApi().sendMessage(_chatId, _text, false, 0, _keyboard);
  } catch (std::exception& e) {
    tools::Logger::Error(TGBOT_ERROR, e.what());
  }
}

void ToDoBot::Bot::EditMessage(int64_t _chatId, int32_t _msgId, const std::string &_text, TgBot::InlineKeyboardMarkup::Ptr _keyboard) {
  try {
    m_Bot.getApi().editMessageText(_text, _chatId, _msgId, 
                                   "", "", false, _keyboard);
  } catch (std::exception& e) {

  }
}