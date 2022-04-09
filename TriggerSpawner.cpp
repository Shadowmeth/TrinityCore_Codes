#include "ScriptMgr.h"
#include "ScriptPCH.h"
#include "Player.h"

#define NPC_ID 857
// THIS IS Arathi Basin Battlemaster

class TriggeredSpawner : public CreatureScript
{
public:
    TriggeredSpawner() : CreatureScript("TriggeredSpawner") { }

    CreatureAI *GetAI(Creature *c) const override
    {
        return new TriggeredSpawnerAI(c);
    }

    struct TriggeredSpawnerAI : public ScriptedAI
    {
        TriggeredSpawnerAI(Creature *cp) : ScriptedAI(cp), spawned{false} { }

        void MoveInLineOfSight(Unit * who) override
        {
            if (who -> GetTypeId() == TYPEID_PLAYER && me -> IsWithinDistInMap(who, 125.0f) && !spawned)
            {
                me -> SummonCreature(NPC_ID, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOritentation()/* Orientation*/, TEMPSUMMON_TIMED_DESPAWN, 10000);
                spawned = true;
            }
        }

        bool spawned;
    };

};

void AddSC_TriggeredSpawner()
{
    new TriggeredSpawner();
}
