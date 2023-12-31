#include "messages.h"
#include "../tools/tools.h"

#include <cstdio>
#include <fstream>

#define TEXT "text"
#define BUTTONS "buttons"
#define QUERY "query"
#define URL "url"
#define ENABLED "enabled"


#define GET_ENABLED [ENABLED].get<bool>()
#define GET_URL [URL].get<std::string>()
#define GET_TEXT [TEXT].get<std::string>()
#define GET_QUERY [QUERY].get<std::string>()

using json = nlohmann::json;

using Languages = ToDoBot::Languages;
using Query = ToDoBot::Query;
using Queries = ToDoBot::Queries;
using Message = ToDoBot::Message;


Languages Languages::s_Instance;
Query Query::s_Instance;

Query &Query::Get() { return s_Instance; }

void Query::Init(const char *_filename) {
  std::ifstream file(_filename);
  if (not file.is_open()) {
    tools::Logger::Error(FILE_ERROR, _filename);
    exit(1);
  }

  json json_data = json::parse(file)[ENGLISH_LANG];
  file.close();
  json main_menu_buttons = json_data[MAIN_MESSAGE][BUTTONS];
  json tasks_menu_buttons = json_data[TASKS_MESSAGE][BUTTONS];
  json water_menu_buttons = json_data[WATER_MESSAGE][BUTTONS];
  json language_buttons = json_data[LANGUAGE_MESSAGE][BUTTONS];

  query_data[(int)Queries::main_menu] = tasks_menu_buttons[0]GET_QUERY;
  query_data[(int)Queries::tasks_menu] = main_menu_buttons[0]GET_QUERY;
  query_data[(int)Queries::water_menu] = main_menu_buttons[1]GET_QUERY;
  query_data[(int)Queries::contact_with_creator] = main_menu_buttons[2]GET_QUERY;
  query_data[(int)Queries::add_100] = water_menu_buttons[2]GET_QUERY;
  query_data[(int)Queries::add_200] = water_menu_buttons[3]GET_QUERY;
  query_data[(int)Queries::add_300] = water_menu_buttons[4]GET_QUERY;
  query_data[(int)Queries::add_400] = water_menu_buttons[5]GET_QUERY;
  query_data[(int)Queries::add_other_volume] = water_menu_buttons[6]GET_QUERY;
  query_data[(int)Queries::add_task] = tasks_menu_buttons[2]GET_QUERY;
  query_data[(int)Queries::send_current_tasks] = tasks_menu_buttons[4]GET_QUERY;
  query_data[(int)Queries::send_completed_tasks] = tasks_menu_buttons[5]GET_QUERY;
  query_data[(int)Queries::language_en] = language_buttons[0]GET_QUERY;
  query_data[(int)Queries::language_ru] = language_buttons[1]GET_QUERY;
}

Queries Query::GetQueryId(const std::string &_q) {
  for (int i = 0; i < query_data.size(); i++) {
    if (query_data[i].starts_with(_q)) { return (Queries)i; }
  }
  return Queries::__MaxNumber;
}

void Languages::Init(const char *_filename) {
  std::ifstream file(_filename);
  if (not file.is_open()) {
    tools::Logger::Error("file error", _filename);
    exit(1);
  }
  json data = json::parse(file);
  file.close();

  InitMessages(data);
}

Languages &Languages::Get() {
  return s_Instance;
}

void Languages::InitMessages(const json &_data) {
  for (auto &language_it : _data.items()) {
    json messages = language_it.value();
    std::unordered_map<std::string, Message> &lang = m_Languages[language_it.key()];

    lang[MAIN_MESSAGE].text = messages[MAIN_MESSAGE]GET_TEXT;
    lang[MAIN_MESSAGE].keyboard = InitKeyboard(messages[MAIN_MESSAGE][BUTTONS]);

    lang[TASKS_MESSAGE].text = messages[TASKS_MESSAGE]GET_TEXT;
    lang[TASKS_MESSAGE].keyboard = InitKeyboard(messages[TASKS_MESSAGE][BUTTONS]);

    lang[WATER_MESSAGE].text = messages[WATER_MESSAGE]GET_TEXT;
    lang[WATER_MESSAGE].keyboard = InitKeyboard(messages[WATER_MESSAGE][BUTTONS]);

    lang[LANGUAGE_MESSAGE].text = messages[LANGUAGE_MESSAGE]GET_TEXT;
    lang[LANGUAGE_MESSAGE].keyboard = InitKeyboard(messages[LANGUAGE_MESSAGE][BUTTONS]);
  }
}

TgBot::InlineKeyboardMarkup::Ptr Languages::InitKeyboard(const json &_data) {
  TgBot::InlineKeyboardMarkup::Ptr kb(new TgBot::InlineKeyboardMarkup);
  for (int i = 0; i < _data.size(); i += 2) {
    std::vector<TgBot::InlineKeyboardButton::Ptr> row;

    if (_data[i]GET_ENABLED) {
      TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
      row.push_back(btn);
      btn->text = _data[i][TEXT];
      btn->callbackData = _data[i][QUERY];
      if (_data[i].contains(URL)) {
        btn->url = _data[i]GET_URL;
      }
    }
    
    if (_data[i + 1]GET_ENABLED) {
      TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
      row.push_back(btn);
      btn->text = _data[i + 1][TEXT];
      btn->callbackData = _data[i + 1][QUERY];
      if (_data[i + 1].contains(URL)) {
        btn->url = _data[i + 1]GET_URL;
      }
    }

    kb->inlineKeyboard.push_back(row);
  }
  return kb;
}

Message Languages::GetMessage(const std::string &_lang, const std::string &_msg) {
  return m_Languages[_lang][_msg];
}