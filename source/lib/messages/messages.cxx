#include "messages.h"
#include "../tools/tools.h"
#include <fstream>

#define JSON_FILENAME "json/languages.json"

using json = nlohmann::json;

ToDoBot::Languages ToDoBot::Languages::s_Instance;
std::unordered_map<std::string, ToDoBot::Lang> ToDoBot::Languages::m_Langs;

void ToDoBot::Languages::Init() {
  std::ifstream file(JSON_FILENAME);
  if (not file.is_open()) {
    tools::Logger::Error("file error", JSON_FILENAME);
    exit(1);
  }
  json data = json::parse(file);
  file.close();

  InitMessages(data);
}

void ToDoBot::Languages::InitMessages(const json &_data) {
  for (auto &it : _data.items()) {
    Lang lang;
    json val = it.value();
    lang.mainMessage.text = val["main-menu"]["text"].get<std::string>().c_str();
    lang.mainMessage.keyboard = InitKeyboard(val["main-menu"]["buttons"]);
    lang.tasksMessage.text = val["tasks"]["text"].get<std::string>().c_str();
    lang.tasksMessage.keyboard = InitKeyboard(val["tasks"]["buttons"]);
    lang.waterMessage.text = val["water"]["text"].get<std::string>().c_str();
    lang.waterMessage.keyboard = InitKeyboard(val["water"]["buttons"]);

    std::cout << it.key() << " : " << it.value() << "\n";
    m_Langs[it.key()] = lang;
  }
}

TgBot::InlineKeyboardMarkup::Ptr ToDoBot::Languages::InitKeyboard(const json &_data) {
  TgBot::InlineKeyboardMarkup::Ptr kb(new TgBot::InlineKeyboardMarkup);
  for (int i = 0; i < _data.size(); i += 2) {
    std::vector<TgBot::InlineKeyboardButton::Ptr> row;

    if (_data[i]["enabled"].get<bool>()) {
      TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
      row.push_back(btn);
      btn->text = _data[i]["text"];
      btn->callbackData = _data[i]["query"];
      if (_data[i].contains("url")) {
        btn->url = _data[i]["url"].get<std::string>();
      }
    }
    
    if (_data[i + 1]["enabled"].get<bool>()) {
      TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton); // ! maybe error
      row.push_back(btn);
      btn->text = _data[i + 1]["text"];
      btn->callbackData = _data[i + 1]["query"];
      if (_data[i + 1].contains("url")) {
        btn->url = _data[i + 1]["url"].get<std::string>();
      }
    }

    kb->inlineKeyboard.push_back(row);
  }
  return kb;
}

ToDoBot::Lang ToDoBot::Languages::GetLanguage(int _id) {

}