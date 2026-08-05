#include "../pch.h"
#include "CharacterRoster.h"
#include <algorithm>
#include <iostream>

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
