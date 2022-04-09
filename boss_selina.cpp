#include "ScriptPCH.h"
#include "CreatureAI.h"
#include "Chat.h"

enum SelinaPHASES
{
	PHASE_1 = 1,
	PHASE_2,
	PHASE_3
};

enum SelinaEVENTS
{
	EVENT_CAST_FIREBALL = 1,
	EVENT_CAST_PYROBLAST,
	EVENT_CAST_WAR_STOMP
};

enum SelinaDATA
{
	DATA_SELINA = 1
};

enum SelinaSPELLS
{
	SPELL_HEROISM	= 32182,
	SPELL_FIREBALL	= 42833,
	SPELL_WAR_STOMP = 25188,
	SPELL_PYROBLAST	= 42891,
	SPELL_BERSERK	= 72525, // Damage done increased by 240%. Attack and casting speeds increased by 160%.

};

class Boss_Selina : public CreatureScript
{
public:

	Boss_Selina() : CreatureScript("Boss_Selina") {	}

	CreatureAI* GetAI(Creature* c) const override 
	{
		return new Boss_Selina_AI(c);
	}

	struct Boss_Selina_AI : public BossAI
	{
		Boss_Selina_AI(Creature* c) : BossAI(c, DATA_SELINA) {	}


		void MeStopMoving(Unit* who)
		{
			AttackStartNoMove(who);
		}

		CastSpellExtraArgs ConstructOPSpell(int bp0)
		{
			CastSpellExtraArgs args;
			args.AddSpellBP0(bp0);
			return args;
		}

		void JustEngagedWith(Unit* who) override
		{
			if (who->GetTypeId() != TYPEID_PLAYER)
				return; // Don't send notification for non-players

			MeStopMoving(who);
			Player* player = who->ToPlayer();
			ChatHandler(player->GetSession()).PSendSysMessage("You have angered Selina!");
			events.SetPhase(PHASE_1);
			DoCast(me, SPELL_HEROISM);
			events.ScheduleEvent(EVENT_CAST_FIREBALL, 4s, PHASE_1);
			BossAI::JustEngagedWith(who);
		}
		
		void UpdateAI(uint32 diff) override
		{
			if (!UpdateVictim())
				return;

			events.Update(diff);

			if (events.IsInPhase(PHASE_1) && (!me->HasAura(SPELL_HEROISM) || HealthBelowPct(51)))
			{
				if (me->HasUnitState(UNIT_STATE_CASTING))
				{
					me->CastStop();
				}
				// Phase 2 begins, remove heroism if it exists already
				me->RemoveAllAuras();
				events.Reset();
				events.SetPhase(PHASE_2);
				events.ScheduleEvent(EVENT_CAST_PYROBLAST, 0s, PHASE_2);
			}

			if (events.IsInPhase(PHASE_2) && HealthBelowPct(35) && !events.IsInPhase(PHASE_3))
			{
				if (me->HasUnitState(UNIT_STATE_CASTING))
				{
					me->CastStop();
				}
				// Phase 3 begins
				events.SetPhase(PHASE_3);
				DoCastSelf(SPELL_BERSERK);
				events.ScheduleEvent(EVENT_CAST_PYROBLAST, 1s, PHASE_3);
				me->SetResistance(SPELL_SCHOOL_NORMAL, INT32_MAX);
			}

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_CAST_FIREBALL:
				{
					// Don't interrupt the cast, let it finish and then schedule event again
					// It might not always be in casting due to this but that's fine
					if (me->HasUnitState(UNIT_STATE_CASTING))
						return;

					DoCast(me->GetVictim(), SPELL_FIREBALL, ConstructOPSpell(10000));
					events.ScheduleEvent(EVENT_CAST_FIREBALL, 4s, PHASE_1);
					break;
				}

				case EVENT_CAST_PYROBLAST:
				{
					if (events.IsInPhase(PHASE_2))
					{
						if (Unit* target = me->SelectNearestPlayer(6.0f))
						{
							if (me->HasUnitState(UNIT_STATE_CASTING))
								me->CastStop();
							DoCast(target, SPELL_WAR_STOMP);
						}

						if (me->HasUnitState(UNIT_STATE_CASTING))
							return;

						DoCast(me->GetVictim(), SPELL_PYROBLAST, ConstructOPSpell(15000));
						events.ScheduleEvent(EVENT_CAST_PYROBLAST, 5s, PHASE_2);

					}
					if (events.IsInPhase(PHASE_3))
					{
						if (me->HasUnitState(UNIT_STATE_CASTING))
							return;

						Unit* random_target = SelectTarget(SelectTargetMethod::Random);
						DoCast(random_target, SPELL_PYROBLAST, ConstructOPSpell(5000));
						events.ScheduleEvent(EVENT_CAST_PYROBLAST, 2s, PHASE_3);
							
					}

					
				}

				}

			}

		}
	};
};

void ADDSC_boss_selina()
{
	new Boss_Selina();
}