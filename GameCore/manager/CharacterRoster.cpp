#include "../pch.h"
#include "CharacterRoster.h"
#include "../model/Warrior.h"
#include "../model/Mage.h"
#include <algorithm>
#include <iostream>
#include <cctype>

/*
  Author: Nguyen Dinh Dung
  Date modified: 2026-08-06
*/

CharacterRoster::CharacterRoster() {}

CharacterRoster::~CharacterRoster() {}

int CharacterRoster::add(std::unique_ptr<Character> character)
{
    if (!character) {
        std::cout << "Loi: Khong the them nhan vat null!" << std::endl;
        return -1;
    }
    if (character->getId() <= 0) {
        std::cout << "Loi: Character ID phai la so nguyen duong!" << std::endl;
        return -1;
    }
    if (character->getName().empty()) {
        std::cout << "Loi: Ten nhan vat khong duoc de trong!" << std::endl;
        return -1;
    }
    if (findById(character->getId()) != nullptr) {
        std::cout << "Loi: Character ID " << character->getId() << " da ton tai!" << std::endl;
        return -1;
    }

    int id = character->getId();
    characters.push_back(std::move(character));
    return id;
}

const Character* CharacterRoster::findById(int charId) const
{
    for (const auto& c : characters) {
        if (c->getId() == charId) {
            return c.get();
        }
    }
    return nullptr;
}

Character* CharacterRoster::findById(int charId)
{
    for (auto& c : characters) {
        if (c->getId() == charId) {
            return c.get();
        }
    }
    return nullptr;
}

std::vector<Character*> CharacterRoster::findByName(const std::string& name) const
{
    std::vector<Character*> result;
    if (name.empty()) return result;

    // Tạo bản lowercase của chuỗi tìm kiếm
    std::string lowerName;
    lowerName.reserve(name.size());
    for (char c : name) {
        lowerName += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    for (const auto& c : characters) {
        // Tạo bản lowercase của tên nhân vật
        std::string charName = c->getName();
        std::string lowerCharName;
        lowerCharName.reserve(charName.size());
        for (char ch : charName) {
            lowerCharName += static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }

        // Kiểm tra substring (case-insensitive)
        if (lowerCharName.find(lowerName) != std::string::npos) {
            result.push_back(c.get());
        }
    }
    return result;
}

bool CharacterRoster::updateWarrior(int charId, const std::string& newName, unsigned int newMaxHp, int newAttackPower)
{
    Character* c = findById(charId);
    if (!c) return false;

    Warrior* warrior = dynamic_cast<Warrior*>(c);
    if (!warrior) return false;

    // Lưu giá trị cũ để rollback nếu cần
    std::string oldName = warrior->getName();
    unsigned int oldMaxHp = warrior->getMaxHp();
    unsigned int oldAttackPower = warrior->getAttackPower();

    if (!warrior->setName(newName)) return false;

    if (!warrior->setMaxHp(newMaxHp)) {
        warrior->setName(oldName); // rollback
        return false;
    }

    if (!warrior->setAttackPower(newAttackPower)) {
        warrior->setName(oldName); // rollback
        warrior->setMaxHp(oldMaxHp);
        return false;
    }

    return true;
}

bool CharacterRoster::updateMage(int charId, const std::string& newName, unsigned int newMaxHp, unsigned int newMaxMana, unsigned int newSpellDamage, unsigned int newManaCost, unsigned int newFallbackDamage)
{
    Character* c = findById(charId);
    if (!c) return false;

    Mage* mage = dynamic_cast<Mage*>(c);
    if (!mage) return false;

    // Lưu giá trị cũ để rollback nếu cần
    std::string oldName = mage->getName();
    unsigned int oldMaxHp = mage->getMaxHp();
    unsigned int oldMaxMana = mage->getMaxMana();
    unsigned int oldSpellDamage = mage->getSpellDamage();
    unsigned int oldManaCost = mage->getManaCost();
    unsigned int oldFallbackDamage = mage->getFallbackDamage();

    if (!mage->setName(newName)) return false;

    if (!mage->setMaxHp(newMaxHp)) {
        mage->setName(oldName);
        return false;
    }

    if (!mage->setMaxMana(newMaxMana)) {
        mage->setName(oldName);
        mage->setMaxHp(oldMaxHp);
        return false;
    }

    if (!mage->setSpellDamage(newSpellDamage)) {
        mage->setName(oldName);
        mage->setMaxHp(oldMaxHp);
        mage->setMaxMana(oldMaxMana);
        return false;
    }

    if (!mage->setManaCost(newManaCost)) {
        mage->setName(oldName);
        mage->setMaxHp(oldMaxHp);
        mage->setMaxMana(oldMaxMana);
        mage->setSpellDamage(oldSpellDamage);
        return false;
    }

    if (!mage->setFallbackDamage(newFallbackDamage)) {
        mage->setName(oldName);
        mage->setMaxHp(oldMaxHp);
        mage->setMaxMana(oldMaxMana);
        mage->setSpellDamage(oldSpellDamage);
        mage->setManaCost(oldManaCost);
        return false;
    }

    return true;
}

bool CharacterRoster::remove(int charId)
{
    for (auto it = characters.begin(); it != characters.end(); ++it) {
        if ((*it)->getId() == charId) {
            characters.erase(it);
            return true;
        }
    }
    return false;
}


bool CharacterRoster::isEmpty() const
{
    return characters.empty();
}

int CharacterRoster::size() const
{
    return static_cast<int>(characters.size());
}

std::vector<Character*> CharacterRoster::getAllCharacters() const
{
    std::vector<Character*> result;
    result.reserve(characters.size());
    for (const auto& c : characters) {
        result.push_back(c.get());
    }
    return result;
}


bool CharacterRoster::hasCharacter(int charId) const
{
    return findById(charId) != nullptr;
}

void CharacterRoster::displayAll() const
{
    if (characters.empty()) {
        std::cout << "(Chua co nhan vat nao)" << std::endl;
        return;
    }

    std::cout << "\n===== DANH SACH NHAN VAT =====" << std::endl;
    int stt = 0;
    for (const auto& c : characters) {
		std::cout << "{STT " << (++stt) <<"} : ";
        c->display();   // Gọi đa hình
    }

    std::cout << "==============================" << std::endl;
}
