#pragma once

// CombatantSlot.h — dữ liệu phiên đấu của một nhân vật
// Tách riêng để tránh circular dependency giữa Character.h và Battle.h
// Character.h include CombatantSlot.h (để dùng trong performAction)
// Battle.h include CombatantSlot.h (để tạo mảng slot)

struct CombatantSlot {
    int characterId;
    int currentHp;
    int currentMana;  // = 0 với Warrior

    CombatantSlot();
    CombatantSlot(int id, int hp, int mana);

    bool isAlive() const;
};
