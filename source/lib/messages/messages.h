#include <unordered_map>
#include <string>
#include <tgbot/tgbot.h>
#include <nlohmann/json.hpp>

namespace ToDoBot {
  struct Lang {
    struct Message {
      const char *text;
      TgBot::InlineKeyboardMarkup::Ptr keyboard;
    } mainMessage, waterMessage, tasksMessage;
  };

  class Languages {
  public:
    Languages(const Languages &) = delete;
    
    static void Init();
    static void InitMessages(const nlohmann::json &_data);
    static TgBot::InlineKeyboardMarkup::Ptr InitKeyboard(const nlohmann::json &_data);
    static Lang GetLanguage(int _id);

  private:
    Languages() {}
    static Languages s_Instance;
    static std::unordered_map<std::string, Lang> m_Langs;
  };
}