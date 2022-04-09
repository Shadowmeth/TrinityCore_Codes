#include "ScriptMgr.h"
#include "ScriptPCH.h"
#include "ScriptedCreature.h"
#include "UnitAI.h"
#include "Player.h"
#include "Unit.h"

#define PHASE_2 2
#define BOSS_DATA 0
#define NPC_ARMY_OF_DEAD 24207	

enum Events
{
	EVENT_OBLITERATE = 1,
	EVENT_DEATH_COIL = 2,
	EVENT_DEATH_STRIKE = 3,
};

enum Spells
{
	SPELL_AOD = 42650,
	SPELL_OBLITERATE = 49020,
	SPELL_DEATH_COIL = 47541,
	SPELL_DEATH_STRIKE = 49998
};

class CustomDeathbringerSaurfang : public CreatureScript
{
public:
	CustomDeathbringerSaurfang() : CreatureScript("CustomDeathbringerSaurfang") {	}

	CreatureAI * GetAI(Creature *cp) const override
	{
		return new DeathbringerSaurfangAI(cp);
	}

	struct DeathbringerSaurfangAI : public BossAI
	{
		DeathbringerSaurfangAI(Creature *cp) : BossAI(cp, BOSS_DATA) {	}

		void Reset() override
		{
			events.Reset();
		}

		void DamageTaken(Unit * victim/*Attacker*/, uint32 & /*Dmg done*/) override
		{
			if (me->HealthBelowPct(50))
			{
				DoCastVictim(SPELL_AOD);
				while (me->HasUnitState(UNIT_STATE_CASTING))
				{
					;
				}
				events.Reset();
				events.SetPhase(2);
				events.ScheduleEvent(EVENT_DEATH_STRIKE, Seconds(5), 0);
				events.ScheduleEvent(EVENT_OBLITERATE, Seconds(3), 0);
				events.ScheduleEvent(EVENT_DEATH_COIL, Seconds(1), 0);	

			}
		}

		void JustEngagedWith(Unit *) override
		{
			events.ScheduleEvent(EVENT_DEATH_STRIKE, Seconds(5), 0, 0);
			events.ScheduleEvent(EVENT_OBLITERATE, Seconds(3), 0, 0);
			events.ScheduleEvent(EVENT_DEATH_COIL, Seconds(1), 0, 0);
		}

		void ExecuteEvent(uint32 eventId) override
		{
			switch (eventId)
			{
				case EVENT_DEATH_STRIKE:
					DoCastVictim(SPELL_DEATH_STRIKE);
					if (!events.IsInPhase(1))
						events.ScheduleEvent(EVENT_DEATH_STRIKE, Seconds(5), 0, 0);
					else
						events.ScheduleEvent(EVENT_DEATH_STRIKE, Seconds(5), 0, 1);

					break;

				case EVENT_OBLITERATE:
					DoCastVictim(SPELL_OBLITERATE);
					if(!events.IsInPhase(1))
						events.ScheduleEvent(EVENT_OBLITERATE, Seconds(3), 0, 0);
					else
						events.ScheduleEvent(EVENT_OBLITERATE, Seconds(3), 0, 1);

					break;
				case EVENT_DEATH_COIL:
					DoCastVictim(SPELL_DEATH_COIL);
					if (!events.IsInPhase(1))
						events.ScheduleEvent(EVENT_DEATH_COIL, Seconds(1), 0, 0);
					else
						events.ScheduleEvent(EVENT_DEATH_COIL, Seconds(1), 0, 1);

					break;

				default:
					break;
			}
		}

	};

};

void AddSC_CustomDeathbringerSaurfang()
{
	new CustomDeathbringerSaurfang();
}