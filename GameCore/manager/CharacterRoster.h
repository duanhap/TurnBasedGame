#pragma once
#include <vector>
#include <memory>
#include "../model/Character.h"

/*
	Author: Nguyen Dinh Dung
	Date modified: 2026-08-06
*/
class CharacterRoster
{
private:
	std::vector<std::unique_ptr<Character>> characters;
public:
	CharacterRoster();
	~CharacterRoster();
	int add(std::unique_ptr<Character> character); // thêm 1 character. trả về character id nếu thành công, -1 nếu thất bại (do trùng id, tên không hợp lệ,...)
	const Character* findById(int charId) const; // Tuyệt đối KHÔNG ĐƯỢC XÓA con trỏ trả về. Tìm nhân vật theo Id, trả về con trỏ tới object nếu thấy, null nếu ko tồn tại.
	Character* findById(int charId); // tìm nhân vật theo Id (phiên bản có thể update), tuyệt đối KHÔNG ĐƯỢC XÓA con trỏ trả về
	std::vector<Character*> findByName(const std::string& name) const; // tìm nhân vật theo tên, trả về vector chứa con trỏ tới tất cả nhân vật có tên gần giống với name. Nếu ko có thì trả về vector rỗng. Tuyệt đối KHÔNG ĐƯỢC XÓA con trỏ trả về.
	bool updateWarrior(int charId, const std::string& newName, unsigned int newMaxHp, int newAttackPower); // cập nhật thông tin nhân vật Warrior theo character.id. trả về true nếu thành công, false nếu thất bại (do ko tìm thấy charId hoặc thông tin mới ko hợp lệ)
	bool updateMage(int charId, const std::string& newName, unsigned int newMaxHp, unsigned int newMaxMana, unsigned int newSpellDamage, unsigned int newManaCost, unsigned int newFallbackDamage); // cập nhật thông tin nhân vật Mage theo character.id. trả về true nếu thành công, false nếu thất bại (do ko tìm thấy charId hoặc thông tin mới ko hợp lệ)
	bool remove(int charId); // xóa nhân vật khỏi roster theo character.id

	bool isEmpty() const; // trả về true nếu ko có nhân vật nào.
	int size() const; // trả về số lượng nhân vật.
	std::vector<Character*> getAllCharacters() const;
	bool hasCharacter(int charId) const; // trả về true nếu roster chứa nhân vật với charId đã cho
	void displayAll() const; // in toàn bộ danh sách nhân vật ra console
};