#include "Player.h"
#include "Group.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "ScriptPCH.h"
#include "Chat.h"
#include <cstdlib>

enum MenuDefines
{
	MENU_DEFINE_GREETINGS_MENU_DB_DEFINE = 16777215
};

enum Items
{
	ITEM_HONEY_GLAZED_HAUNCES = 154882
};

class ExtendedGossipNPC : public CreatureScript
{
public:
	ExtendedGossipNPC() : CreatureScript("ExtendedGossipNPC")  {	}

	CreatureAI * GetAI(Creature *c) const override
	{
		return new ExtendedGossipNPCAI(c);
	}

	struct ExtendedGossipNPCAI : public ScriptedAI
	{
		ExtendedGossipNPCAI(Creature *cp) : ScriptedAI(cp)	{	}

		bool GossipHello(Player *player) override
		{
			if (player->IsInCombat())
			{
				ChatHandler(player->GetSession()).PSendSysMessage("You are in combat, fucking retard");
				return false;
			}

			AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Click me to get food!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1, "You get twice the food\nof your value ", 0, true);
			AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Close...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
			SendGossipMenuFor(player, MENU_DEFINE_GREETINGS_MENU_DB_DEFINE, me->GetGUID());
			return true;

		}

		bool GossipSelect(Player * player, uint32/*MenuID*/, uint32 GossipActionList) override
		{
			uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(GossipActionList);
			ClearGossipMenuFor(player);
			switch (action)
			{
				case GOSSIP_ACTION_INFO_DEF + 1:	
					/*PUT LOGIC HERE!!!*/
					CloseGossipMenuFor(player);
					break;
					
				case GOSSIP_ACTION_INFO_DEF + 2:
					CloseGossipMenuFor(player);
					break;
			}
			return true;
		}

		int IsValid(char const* c_str)
		{
			char const * str = c_str;

			while (*str)
			{
				if (((*str) > '9') || ((*str) < '0'))
					return 0;
				str++;
			}
			return 1;

		}

		bool GossipSelectCode(Player *player, uint32 /*Sender*/, uint32 GossipActionList/*action*/, char const * UserInput/*Code*/) override
		{
			uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(GossipActionList);
			switch (action)
			{
				case GOSSIP_ACTION_INFO_DEF + 1:
				{
					int val = std::atoi(UserInput);
					if (val < 0 || !IsValid(UserInput)) {
						ChatHandler(player->GetSession()).PSendSysMessage("You entered an incorrect value, fucking retard!");
						return false;
					}

					player->AddItem(ITEM_HONEY_GLAZED_HAUNCES, val * 2);
					ChatHandler(player->GetSession()).PSendSysMessage("Honey Glazed Haunches have been added for you!");

				}
					break;

				default:
					break;
			}

			return true;
		}

	};

};

void AddSC_ExtendedGossipNPC()
{
	new ExtendedGossipNPC();
}