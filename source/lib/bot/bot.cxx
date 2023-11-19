#include "bot.h"
#include "../tools/tools.h"

#include <exception>

#define BACK_TO_MAIN_MENU "back"

#define MAIN_KEYBOARD "main-kb"
#define DRINK_WATER "drink-water"
#define TASKS "tasks"

#define WATER_KEYBOARD "water-kb"
#define ADD_100_ML "add-100"
#define ADD_200_ML "add-200"
#define ADD_300_ML "add-300"
#define ADD_400_ML "add-400"
#define ADD_ANOTHER "add-another"

#define TASKS_KEYBOARD "tasks-kb"
#define NEW_TASK "add-new-task"
#define SEND_UNCOMPLETED_TASKS "send-uc-tasks"
#define SEND_COMPLETED_TASKS "send-c-tasks"

ToDoBot::Bot::Bot() : m_Bot(tools::GetToken()) {
  InitCommands();
  InitKeyboards();
  InitCallbacks();
}

void ToDoBot::Bot::InitCallbacks() {
  auto IsMessageCommand = [](const std::string &_message) {
    if (
      _message.starts_with("/start") 
      or _message.starts_with("/menu")
      ) { return true; }
    return false;
  };

  auto OnStart = [&](TgBot::Message::Ptr _message) {
    m_Bot.getApi().deleteMessage(_message->chat->id, _message->messageId);
    m_Bot.getApi().sendMessage(_message->chat->id, "Hi, " + _message->from->username + "!");
    std::string request = "INSERT INTO Users (UserID, IsAdmin) VALUES (" + 
                          std::to_string(_message->from->id) + 
                          ", 0)";
    tools::SQLite3::Get().ExecuteRequest(request.c_str(), nullptr);
  };

  auto OnMenu = [&](Message _message) {
    m_Bot.getApi().deleteMessage(_message->chat->id, _message->messageId);
    m_Bot.getApi().sendMessage(_message->chat->id, "Main menu:", false, 0, keyboards[MAIN_KEYBOARD]);
  };

  auto AnswerToMessage = [&](Message _message) {
    if (IsMessageCommand(_message->text)) { return; }
    m_Bot.getApi().deleteMessage(_message->chat->id, _message->messageId);
    // m_Bot.getApi().sendMessage(_message->chat->id, "Your message is: " + _message->text, false, _message->messageId);
  };

  auto OnCallbackQuery = [&](TgBot::CallbackQuery::Ptr _query) {
    if (_query->data.starts_with(DRINK_WATER)) {
      m_Bot.getApi().editMessageText("Edited", _query->message->chat->id,
                                     _query->message->messageId, "", "", false,
                                     keyboards[WATER_KEYBOARD]);
    } else if (_query->data.starts_with(TASKS)) {
      m_Bot.getApi().editMessageText("What do you want to do?", _query->message->chat->id,
                                     _query->message->messageId, "", "", false,
                                     keyboards[TASKS_KEYBOARD]);
    } else if (_query->data.starts_with(BACK_TO_MAIN_MENU)) {
      m_Bot.getApi().editMessageText("Main menu:", _query->message->chat->id,
                                     _query->message->messageId, "", "", false,
                                     keyboards[MAIN_KEYBOARD]);
    }
  };

  m_Bot.getEvents().onCommand("start", OnStart);
  m_Bot.getEvents().onCommand("menu", OnMenu);
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

  m_Bot.getApi().setMyCommands(commands);
}

void ToDoBot::Bot::InitKeyboards() {
  InitMainKB();
  InitWaterKB();
  InitTasksKB();
}

void ToDoBot::Bot::InitMainKB() {
  TgBot::InlineKeyboardMarkup::Ptr mainKeyboard(new TgBot::InlineKeyboardMarkup);
  
  std::vector<TgBot::InlineKeyboardButton::Ptr> row0;
  {
    TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
    row0.push_back(btn);
    btn->text = "Tasks";
    btn->callbackData = TASKS;
  } {
    TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
    row0.push_back(btn);
    btn->text = "Drink water";
    btn->callbackData = DRINK_WATER;
  }

  std::vector<TgBot::InlineKeyboardButton::Ptr> row1;
  {
    TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
    row1.push_back(btn);
    btn->text = "Contact with the creator";
    btn->url = "tg://user?id=1298547601";
  }
  mainKeyboard->inlineKeyboard.push_back(row0);
  mainKeyboard->inlineKeyboard.push_back(row1);
  keyboards[MAIN_KEYBOARD] = mainKeyboard;
}

void ToDoBot::Bot::InitWaterKB() {
  TgBot::InlineKeyboardMarkup::Ptr drinkWaterKeyboard(new TgBot::InlineKeyboardMarkup);

  std::vector<TgBot::InlineKeyboardButton::Ptr> row;
  std::vector<TgBot::InlineKeyboardButton::Ptr> row0;
  std::vector<TgBot::InlineKeyboardButton::Ptr> row1;
  std::vector<TgBot::InlineKeyboardButton::Ptr> row2;
  {
    TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
    row.push_back(btn);
    btn->text = "<< Main menu";
    btn->callbackData = BACK_TO_MAIN_MENU;
  } {
    TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
    row0.push_back(btn);
    btn->text = "Add 100 ml";
    btn->callbackData = ADD_100_ML;
  } {
    TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
    row0.push_back(btn);
    btn->text = "Add 200 ml";
    btn->callbackData = ADD_200_ML;
  } {
    TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
    row1.push_back(btn);
    btn->text = "Add 300 ml";
    btn->callbackData = ADD_300_ML;
  } {
    TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
    row1.push_back(btn);
    btn->text = "Add 400 ml";
    btn->callbackData = ADD_400_ML;
  } {
    TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
    row2.push_back(btn);
    btn->text = "Add another volume...";
    btn->callbackData = ADD_ANOTHER;
  }

  drinkWaterKeyboard->inlineKeyboard.push_back(row);
  drinkWaterKeyboard->inlineKeyboard.push_back(row0);
  drinkWaterKeyboard->inlineKeyboard.push_back(row1);
  drinkWaterKeyboard->inlineKeyboard.push_back(row2);
  keyboards[WATER_KEYBOARD] = drinkWaterKeyboard;
}

void ToDoBot::Bot::InitTasksKB() {
  TgBot::InlineKeyboardMarkup::Ptr tasksKeyboard(new TgBot::InlineKeyboardMarkup);
  
  std::vector<TgBot::InlineKeyboardButton::Ptr> row0;
  {
    TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
    row0.push_back(btn);
    btn->text = "Add task";
    btn->callbackData = NEW_TASK;
  }

  std::vector<TgBot::InlineKeyboardButton::Ptr> row1;
  {
    TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
    row1.push_back(btn);
    btn->text = "Send current tasks";
    btn->callbackData = SEND_UNCOMPLETED_TASKS;
  } {
    TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
    row1.push_back(btn);
    btn->text = "Send completed tasks";
    btn->callbackData = SEND_COMPLETED_TASKS;
  }

  tasksKeyboard->inlineKeyboard.push_back(row0);
  tasksKeyboard->inlineKeyboard.push_back(row1);
  keyboards[TASKS_KEYBOARD] = tasksKeyboard;
}

void ToDoBot::Bot::Run() {
  try {
    tools::Logger::DebuggingInfo("Bot username:");
    tools::Logger::DebuggingInfo(m_Bot.getApi().getMe()->username.c_str());

    m_Bot.getApi().deleteWebhook();

    LongPoll longPoll(m_Bot);
    
    while (true) {
      longPoll.start();
    }
  } catch (std::exception& e) {
    tools::Logger::Error(TGBOT_ERROR, e.what());
  }
}
