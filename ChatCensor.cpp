#include "ScriptPCH.h"
#include "Channel.h"
 
class System_Censure : public PlayerScript
{
public:
        System_Censure() : PlayerScript("System_Censure") {}
 
        void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg)
        {
                CheckMessage(player, msg, lang, NULL, NULL, NULL, NULL);
        }
 
        void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Player* receiver)
        {
                CheckMessage(player, msg, lang, receiver, NULL, NULL, NULL);
        }
 
        void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Group* group)
        {
                CheckMessage(player, msg, lang, NULL, group, NULL, NULL);
        }
 
        void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Guild* guild)
        {
                CheckMessage(player, msg, lang, NULL, NULL, guild, NULL);
        }
 
        void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Channel* channel)
        {
                CheckMessage(player, msg, lang, NULL, NULL, NULL, channel);
        }
 
void CheckMessage(Player* player, std::string& msg, uint32 lang, Player* /*receiver*/, Group* /*group*/, Guild* /*guild*/, Channel* channel)
{
    //if (player->isGameMaster() || lang == LANG_ADDON)
            //return;
 
    // transform to lowercase (for simpler checking)
    std::string lower = msg;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
 
    uint8 cheksSize = 11;//Change these if you want to add more words to the array.
    std::string checks[11];//Change these if you want to add more words to the array.
    // Strony (Sites)
    checks[0] ="Styler is gay";
    checks[1] ="Styler is Homosexual";
    checks[2] ="Infusion-WoW Sucks";
    checks[3] ="Fuck Styler";
    checks[4] ="I hate Styler";
    checks[5] ="Styler is retarded";
    checks[6] ="Screw Styler";
    checks[7] ="wow-";
    checks[8] ="-wow";
    checks[9] =".pl";
        checks[10] ="lumiawow is better than Deception";
 
    for (int i = 0; i < cheksSize; ++i)
        if (lower.find(checks[i]) != std::string::npos)
        {
            msg = "";
            ChatHandler(player->GetSession()).PSendSysMessage("Advertising and vulgar behavior is not allowed!");
            return;
        }
}
};
 
void AddSC_System_Censure()
{
    new System_Censure();
}