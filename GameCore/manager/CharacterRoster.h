#pragma once
#include <vector>
#include <memory>
#include "../model/Character.h"

/*
	Author: Nguyen Dinh Dung
	Date modified: 2026-08-09
*/
class CharacterRoster
{
private:
	/// Số lượng nhân vật tối đa mà roster có thể chứa. Nếu vượt quá, add() sẽ trả về -1
	const static int MAX_SIZE = 100;

	std::vector<std::unique_ptr<Character>> characters;
public:
	CharacterRoster();
	~CharacterRoster();
	/// thêm 1 character. trả về character id nếu thành công, -1 nếu thất bại (do trùng id, tên không hợp lệ,...)
	int add(std::unique_ptr<Character> character); 
	/// Tuyệt đối KHÔNG ĐƯỢC XÓA con trỏ trả về. Tìm nhân vật theo Id, trả về con trỏ tới object nếu thấy, null nếu ko tồn tại.
	const Character* findById(int charId) const; 
	/// tìm nhân vật theo Id (phiên bản có thể update), tuyệt đối KHÔNG ĐƯỢC XÓA con trỏ trả về
	Character* findById(int charId); 
	/// tìm nhân vật theo tên, trả về vector chứa con trỏ tới tất cả nhân vật có tên gần giống với name. Nếu ko có thì trả về vector rỗng. Tuyệt đối KHÔNG ĐƯỢC XÓA con trỏ trả về.
	std::vector<Character*> findByName(const std::string& name) const;
	bool updateWarrior(int charId, const std::string& newName, unsigned int newMaxHp, int newAttackPower); // cập nhật thông tin nhân vật Warrior theo character.id. trả về true nếu thành công, false nếu thất bại (do ko tìm thấy charId hoặc thông tin mới ko hợp lệ)
	bool updateMage(int charId, const std::string& newName, unsigned int newMaxHp, unsigned int newMaxMana, unsigned int newSpellDamage, unsigned int newManaCost, unsigned int newFallbackDamage); // cập nhật thông tin nhân vật Mage theo character.id. trả về true nếu thành công, false nếu thất bại (do ko tìm thấy charId hoặc thông tin mới ko hợp lệ)
	bool updateArcher(int charId, const std::string& newName, unsigned int newMaxHp, unsigned int newNormalDamage, unsigned int newCriticalDamage);
	bool updateHealer(int charId, const std::string& newName, unsigned int newMaxHp, unsigned int newHealingPower);
	/// xóa nhân vật khỏi roster theo character.id
	bool remove(int charId); 
	/// trả về true nếu ko có nhân vật nào.
	bool isEmpty() const; 
	/// trả về số lượng nhân vật hiện có.
	int size() const; 
	std::vector<Character*> getAllCharacters() const;
	/// trả về true nếu roster chứa nhân vật với charId đã cho
	bool hasCharacter(int charId) const;
	/// in toàn bộ danh sách nhân vật ra console
	void displayAll() const;
};