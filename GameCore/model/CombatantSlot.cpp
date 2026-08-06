#include "../pch.h"
#include "CombatantSlot.h"

CombatantSlot::CombatantSlot()
    : characterId(0), currentHp(0), currentMana(0)
{}

CombatantSlot::CombatantSlot(int id, int hp, int mana)
    : characterId(id), currentHp(hp), currentMana(mana)
{}

bool CombatantSlot::isAlive() const {
    return currentHp > 0;
}
