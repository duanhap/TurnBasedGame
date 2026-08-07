# Danh Sách Test Case: Luồng Console & Xử Lý Input

Tài liệu này định nghĩa bộ test case để kiểm thử luồng console và xử lý dữ liệu đầu vào (Input Validation) của **Turn-Based Adventure Game**. Mục tiêu là đảm bảo giao diện console hoạt động ổn định, không bị crash, không bị lặp vô hạn trước mọi dạng dữ liệu nhập (hợp lệ, không hợp lệ, giá trị biên, tràn số và chuỗi rỗng).

---

## 1. Kiểm Thử Input Tại Menu Chính (Main Menu)
Các test case này kiểm tra tính hợp lệ khi người dùng nhập lựa chọn tại menu cấp 1 của ứng dụng.

| Mã Test Case | Kịch Bản / Mục Tiêu | Dữ Liệu Nhập | Kết Quả Mong Đợi | Trạng Thái |
| :--- | :--- | :--- | :--- | :--- |
| **TC-MC-001** | Chọn chức năng hợp lệ | `1`, `2`, `3` hoặc `4` | Chuyển sang sub-menu tương ứng hoặc lưu dữ liệu và thoát game. | **PASS** |
| **TC-MC-002** | Chọn số nguyên ngoài phạm vi | `5` hoặc `0` | In ra thông báo: `[LOI] Vui long nhap so trong khoang [1, 4].` và yêu cầu nhập lại. | **PASS** |
| **TC-MC-003** | Nhập sai kiểu dữ liệu (chữ/ký tự) | `"abc"`, `"!@#"`, hoặc `"3.14"` | In ra thông báo: `[LOI] Input khong hop le (phai la so nguyen). Vui long nhap lai.` và yêu cầu nhập lại. | **PASS** |
| **TC-MC-004** | Nhập trống / chỉ bấm Enter | `[Enter]` | Giữ nguyên dấu nhắc, tiếp tục đợi người dùng nhập, không crash hay lặp vô tận. | **PASS** |
| **TC-MC-005** | Nhập số quá lớn gây tràn số (Overflow) | `9999999999` (vượt quá INT_MAX) | In ra thông báo: `[LOI] Input khong hop le (phai la so nguyen). Vui long nhap lai.` và yêu cầu nhập lại. | **PASS** |
| **TC-MC-006** | Dùng tổ hợp phím kết thúc luồng nhập (EOF) | `Ctrl + Z` (trên Windows) | In ra thông báo: `[LOI] Phat hien ket thuc luong nhap (EOF). Thoat chuong trinh.` và thoát game an toàn với mã 0. | **PASS** |

---

## 2. Kiểm Thử Sub-menu Character Roster (Quản Lý Nhân Vật)
Các test case này kiểm tra các thao tác trong menu Character Roster (hiển thị, thêm, sửa, xóa, tìm kiếm).

### Thêm Nhân Vật Mới (Add Character)
| Mã Test Case | Kịch Bản / Mục Tiêu | Dữ Liệu Nhập | Kết Quả Mong Đợi | Trạng Thái |
| :--- | :--- | :--- | :--- | :--- |
| **TC-RS-001** | Thêm Warrior hợp lệ | Loại: `1`<br>ID: `10`<br>Tên: `"Thor"`<br>MaxHP: `300`<br>AttackPower: `45` | In ra thông báo: `[OK] Da them Warrior: Thor (ID=10)` và quay lại menu Roster. | **PASS** |
| **TC-RS-002** | Thêm Mage hợp lệ | Loại: `2`<br>ID: `11`<br>Tên: `"Gandalf"`<br>MaxHP: `200`<br>MaxMana: `150`<br>SpellDamage: `60`<br>ManaCost: `30`<br>FallbackDamage: `15` | In ra thông báo: `[OK] Da them Mage: Gandalf (ID=11)` và quay lại menu Roster. | **PASS** |
| **TC-RS-003** | Chọn loại nhân vật không hợp lệ | Loại: `3` (chỉ cho phép 1-2) | In ra thông báo: `[LOI] Vui long nhap so trong khoang [1, 2].` và yêu cầu nhập lại loại nhân vật. | **PASS** |
| **TC-RS-004** | Trùng ID nhân vật | ID: `10` (nếu ID 10 đã có) | In ra thông báo: `Loi: Character ID 10 da ton tai!` và hiển thị `[LOI] Khong the them nhan vat...` | **PASS** |
| **TC-RS-005** | Nhập ID âm hoặc bằng 0 | ID: `-1` hoặc `0` | In ra thông báo yêu cầu nhập lại số trong khoảng `[1, 2147483647]`. | **PASS** |
| **TC-RS-006** | Nhập tên rỗng hoặc toàn dấu cách | Tên: `"   "` hoặc `[Enter]` | Cắt khoảng trắng đầu cuối, báo lỗi: `[LOI] Noi dung nhap vao khong duoc de trong. Vui long nhap lai.` | **PASS** |
| **TC-RS-007** | Nhập MaxHP không hợp lệ | MaxHP: `-100` hoặc `0` | Báo lỗi khoảng giá trị và yêu cầu nhập lại MaxHP. | **PASS** |
| **TC-RS-008** | Nhập AttackPower của Warrior sai | AttackPower: `-10` hoặc `0` | Báo lỗi khoảng giá trị và yêu cầu nhập lại AttackPower. | **PASS** |
| **TC-RS-009** | Nhập các chỉ số của Mage sai | MaxMana: `0` hoặc SpellDamage: `-5` | Báo lỗi khoảng giá trị cho chỉ số cụ thể đó và yêu cầu nhập lại. | **PASS** |

### Chỉnh Sửa, Xóa & Tìm Kiếm Nhân Vật
| Mã Test Case | Kịch Bản / Mục Tiêu | Dữ Liệu Nhập | Kết Quả Mong Đợi | Trạng Thái |
| :--- | :--- | :--- | :--- | :--- |
| **TC-RS-010** | Chỉnh sửa: ID không tồn tại | ID: `999` (chưa được tạo) | Báo lỗi: `[LOI] Khong tim thay nhan vat co ID = 999` và quay lại menu Roster. | **PASS** |
| **TC-RS-011** | Chỉnh sửa: Nhập chỉ số mới | ID: `10` | Yêu cầu nhập Tên mới, MaxHP mới và các chỉ số phụ tùy loại nhân vật (Warrior/Mage). | **FAIL** (Lưu ý: Logic nhập và cập nhật chỉ số phụ đang bị comment trong `Menu.cpp` dòng 156-166) |
| **TC-RS-012** | Xóa nhân vật: ID tồn tại | ID: `10` | Báo thành công: `[OK] Da xoa nhan vat ID = 10 va loai khoi moi Team.` | **PASS** |
| **TC-RS-013** | Xóa nhân vật: ID không tồn tại | ID: `999` | Báo lỗi: `[LOI] Khong tim thay nhan vat co ID = 999` | **PASS** |
| **TC-RS-014** | Tìm kiếm: ID không tồn tại | ID: `999` | Báo tin nhắn: `[THONG BAO] Khong tim thay nhan vat co ID = 999` | **PASS** |

---

## 3. Kiểm Thử Sub-menu Team (Quản Lý Đội Hình)
Các test case này kiểm tra các thao tác trong menu Team (tạo team, đổi tên, xóa team, thêm/xóa nhân vật vào team).

| Mã Test Case | Kịch Bản / Mục Tiêu | Dữ Liệu Nhập | Kết Quả Mong Đợi | Trạng Thái |
| :--- | :--- | :--- | :--- | :--- |
| **TC-TM-001** | Tạo Team hợp lệ | ID: `1`<br>Tên: `"Alliance"` | In ra thông báo: `[OK] Da tao Team "Alliance" (ID=1)`. | **PASS** |
| **TC-TM-002** | Tạo Team: Trùng ID Team | ID: `1` (nếu ID 1 đã tồn tại) | Báo lỗi: `Loi: Team ID 1 da ton tai!` và hiển thị `[LOI] Khong the tao Team...` | **PASS** |
| **TC-TM-003** | Tạo Team: Trùng tên Team | Tên: `"Alliance"` (nếu đã có) | Báo lỗi: `Loi: Ten Team 'Alliance' da ton tai!` và hiển thị `[LOI] Khong the tao Team...` | **PASS** |
| **TC-TM-004** | Tạo Team: Tên rỗng/toàn dấu cách | Tên: `"   "` | Báo lỗi: `[LOI] Noi dung nhap vao khong duoc de trong. Vui long nhap lai.` | **PASS** |
| **TC-TM-005** | Tạo Team: Vượt giới hạn tối đa | Thêm team thứ 11 | Báo lỗi: `Loi: Khong the tao them Team. Da dat gioi han toi da 10 Teams!` | **PASS** |
| **TC-TM-006** | Đổi tên Team hợp lệ | ID: `1`<br>Tên mới: `"Horde"` | Báo thành công: `[OK] Da doi ten Team ID=1 thanh "Horde"`. | **PASS** |
| **TC-TM-007** | Đổi tên Team: Trùng tên | ID: `1`<br>Tên mới: `"Horde"` (đã bị team khác dùng) | Báo lỗi: `Loi: Ten Team 'Horde' da duoc su dung boi Team khac!` | **PASS** |
| **TC-TM-008** | Thêm nhân vật: ID nhân vật không có | Team ID: `1`<br>ID nhân vật: `999` | Báo lỗi: `Loi: ID nhan vat 999 khong ton tai trong Character Roster!` | **PASS** |
| **TC-TM-009** | Thêm nhân vật: Trùng trong team | Team ID: `1`<br>ID nhân vật: `10` (đã có trong team) | Báo lỗi: `Loi: Nhan vat ID 10 da co trong Team!` | **PASS** |
| **TC-TM-010** | Thêm nhân vật: Vượt giới hạn 5 người | Thêm thành viên thứ 6 | Báo lỗi: `Loi: Team da dat toi da 5 nhan vat!` | **PASS** |

---

## 4. Kiểm Thử Sub-menu Battle (Trận Đấu)
Các test case này kiểm tra việc thiết lập trận đấu và thực hiện hành động lượt của từng nhân vật trong trận.

| Mã Test Case | Kịch Bản / Mục Tiêu | Dữ Liệu Nhập | Kết Quả Mong Đợi | Trạng Thái |
| :--- | :--- | :--- | :--- | :--- |
| **TC-BT-001** | Chọn hai Team hợp lệ | ID Team A: `1` (có người)<br>ID Team B: `2` (có người) | Báo thành công: `[OK] Da chon doi A: "Alliance" vs doi B: "Horde"`. | **PASS** |
| **TC-BT-002** | Chọn Team: ID không tồn tại | ID Team A: `999` | Báo lỗi: `[LOI] Khong tim thay Team ID=999` và quay lại menu Battle. | **PASS** |
| **TC-BT-003** | Chọn Team: Hai Team giống nhau | ID Team A: `1`<br>ID Team B: `1` | Báo lỗi: `[LOI] Khong the chon (Team rong, cung mot Team, hoac nhan vat trung nhau).` | **PASS** |
| **TC-BT-004** | Chọn Team: Team không có nhân vật | ID Team A: `1` (đội rỗng) | Báo lỗi: `Loi: Doi A khong co nhan vat nao!` | **PASS** |
| **TC-BT-005** | Chọn Team: Trùng nhân vật giữa 2 đội | Team A (có ID 10)<br>Team B (có ID 10) | Báo lỗi: `Loi: Co nhan vat bi trung lap giua hai doi!` | **PASS** |
| **TC-BT-006** | Bắt đầu trận đấu: Khi chưa chọn đội | Lựa chọn `2` (Start) trước `1` | Trả về false, báo lỗi: `[LOI] Chua chon du hai Team hoac tran dau da dang dien ra.` | **FAIL** (Lưu ý: Logic bắt đầu trận đấu UI đang bị comment trong `Menu.cpp` dòng 390-397) |
| **TC-BT-007** | Hành động: Chọn target không tham chiến | Target ID: `999` | Báo hành động không hợp lệ. | **FAIL** (Lưu ý: Logic lượt đấu đang bị comment trong `Menu.cpp` dòng 401-435) |
| **TC-BT-008** | Hành động: Chọn target cùng team mình | Target ID: `10` (đồng đội) | Báo lỗi xác thực mục tiêu, yêu cầu chọn lại đối thủ. | **FAIL** (Lưu ý: Logic lượt đấu đang bị comment trong `Menu.cpp` dòng 401-435) |
| **TC-BT-009** | Hành động: Chọn target đã chết (HP = 0) | Target ID: `11` (đối thủ đã chết) | Báo lỗi xác thực mục tiêu, yêu cầu chọn lại đối thủ còn sống. | **FAIL** (Lưu ý: Logic lượt đấu đang bị comment trong `Menu.cpp` dòng 401-435) |

---

## 5. Tổng Hợp Đánh Giá & Phát Hiện Lỗi
Trong quá trình thiết kế test case và kiểm tra mã nguồn thực tế:
1. **Các phần code bị comment trong `Menu.cpp`:**
   * **Cập nhật chỉ số khi sửa nhân vật (TC-RS-011):** Các câu lệnh hỏi nhập và cập nhật chỉ số phụ (`attackPower` hay `maxMana`,...) hiện đang bị comment lại, chỉ cho phép đổi Tên và MaxHP.
   * **Luồng trận đấu (TC-BT-006 đến TC-BT-009):** Toàn bộ logic tương tác trận đấu trong `Menu::doStartBattle()` và `Menu::doPerformAction()` đang bị comment, dẫn đến không thể kiểm thử việc nhập target khi đấu trực tiếp trên UI console.
2. **Tính năng tiện ích xử lý dòng nhập:**
   * Các hàm `Menu::readInt()` và `Menu::readNonEmptyString()` được viết rất tốt, xử lý dọn dẹp bộ đệm lỗi (`cin.clear()` và `cin.ignore()`) chuẩn xác, ngăn được tình trạng lặp vô hạn hoặc crash do nhập sai định dạng hoặc EOF.
