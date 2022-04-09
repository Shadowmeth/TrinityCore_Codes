#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "Chat.h"

class NPC_TRIGGER : public CreatureScript
{
public:
	NPC_TRIGGER() : CreatureScript("NPC_TRIGGER")	{	}

	CreatureAI * GetAI(Creature *cp) const override
	{
		return new NPC_TRIGGER_AI(cp);
	}

	struct NPC_TRIGGER_AI : public ScriptedAI
	{
		NPC_TRIGGER_AI(Creature *c) : ScriptedAI(c) {	}
		
		void MoveInLineOfSight(Unit * who) override
		{
			if (!who)
				return;
			if (who->GetTypeId() == TYPEID_PLAYER && me->IsWithinDistInMap(who, 125.0f) && !m_notificationSent) // Both if's are same, different in m_notificationSent condition
			{
				m_notificationSent = true;
				Player *player = who->ToPlayer();
				ChatHandler(player->GetSession()).PSendSysMessage("You are now within 125 yards of the boss!");
				return;
			}

			if (who->GetTypeId() == TYPEID_PLAYER && !me->IsWithinDistInMap(who, 125.0f) && m_notificationSent)
			{
				//Add Logic here to execute when the player is 125 or more yards away from the Creature
				m_notificationSent = false;
				return;
			}

		}

		private:
			bool m_notificationSent = false;

	};

};

void AddSC_NPC_TRIGGER()
{
	new NPC_TRIGGER();
}
