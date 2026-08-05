#pragma once
#include <vector>
#include <memory>
#include "../model/Character.h"

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
	bool remove(int charId); // xóa nhân vật khỏi roster theo character.id

	bool isEmpty() const; // trả về true nếu ko có nhân vật nào.
	int size() const; // trả về số lượng nhân vật.
	std::vector<Character*> getAllCharacters() const;
	bool hasCharacter(int charId) const; // trả về true nếu roster chứa nhân vật với charId đã cho
	void displayAll() const; // in toàn bộ danh sách nhân vật ra console
};