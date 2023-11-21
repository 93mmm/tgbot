#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <tgbot/tgbot.h>
#include <nlohmann/json.hpp>

#define MAIN_MESSAGE "main-menu"
#define TASKS_MESSAGE "tasks"
#define WATER_MESSAGE "water"
#define LANGUAGE_MESSAGE "languages"

namespace ToDoBot {
enum class Queries {
  main_menu,
  tasks_menu,
  water_menu,
  contact_with_creator,
  add_100,
  add_200,
  add_300,
  add_400,
  add_other_volume,
  add_task,
  send_current_tasks,
  send_completed_tasks,
  language_ru,
  language_en,

  // ! add other queries here
  __MaxNumber
};

class Query {
public:
  Query(const Query &) = delete;
  static Query &Get();
  void Init();
  std::string GetQuery(Queries _q);

private:
  Query() : query_data((int)Queries::__MaxNumber) {}
  std::vector<std::string> query_data;

  static Query s_Instance;
};
}

namespace ToDoBot {
  struct Message {
    std::string text;
    TgBot::InlineKeyboardMarkup::Ptr keyboard;
  };

  class Languages {
  public:
    Languages(const Languages &) = delete;

    static Languages &Get();

    void Init();
    Message GetMessage(const std::string &_lang, const std::string &_msg);

  private:
    Languages() {}
    static Languages s_Instance;
    static Queries s_Queries;
    std::unordered_map<std::string, std::unordered_map<std::string, Message> > m_Languages;

    void InitMessages(const nlohmann::json &_data);
    TgBot::InlineKeyboardMarkup::Ptr InitKeyboard(const nlohmann::json &_data);
  };
}