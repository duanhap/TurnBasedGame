# TURN-BASED ADVENTURE GAME MOCK PROJECT

Chương trình console mô phỏng trận đấu theo lượt giữa hai đội nhân vật (C++11).

## ⚙️ Môi trường & Công cụ Phát triển (Compiler & IDE Specs)
Để đảm bảo nhất quán cho tất cả thành viên trong nhóm khi build và chạy code:
- **Chuẩn C++**: ISO C++11 (`-std=c++11` đối với GCC / `/std:c++11` đối với MSVC).
- **Trình biên dịch (Compiler)**:
  - GCC / MinGW g++ 8.0 trở lên (Linux / Windows MinGW).
  - MSVC v143 / v145 (Microsoft Visual C++ đi kèm Visual Studio 2022).
- **IDE / Editor khuyên dùng**: Visual Studio 2022, VS Code (với C/C++ Extension Pack), CLion.
- **Hệ điều hành**: Windows 10/11, Linux, macOS.

---

## 📁 Cấu trúc Thư mục Dự án
```text
TurnBasedGame/
├── include/       # Header files (.h) - Chứa định nghĩa Lớp & Interface
├── src/           # Implementation files (.cpp) - Chứa hiện thực phương thức
├── data/          # File dữ liệu mẫu (characters.txt, teams.txt)
├── tests/         # Test cases & unit tests
├── Makefile       # Biên dịch bằng GCC / MinGW g++
├── .gitignore     # Loại bỏ các file rác build Visual Studio & GCC
├── README.md      # Tài liệu hướng dẫn & quy chuẩn nhóm
└── main.cpp       # Điểm khởi chạy chính của ứng dụng
```

---

## 🌿 Quy trình Git & Đặt tên Nhánh (Git Workflow)
- **Nhánh chính**: `main` (mọi code hoàn thiện phải được PR và Merge vào `main`).
- **Đặt tên nhánh tính năng**: `feature/T-<Task_ID>-<tên-task>`
  - Ví dụ: `feature/T-02-character-class`, `feature/T-05-character-roster`
- **Quy tắc Commit Message**: `[T-<Task_ID>] <Mô tả ngắn gọn>`
  - Ví dụ: `[T-01] Create project skeleton and build scripts`

---

## 🛠️ Hướng dẫn Biên dịch và Chạy

### 1. Biên dịch bằng GCC (g++ / Makefile)
```bash
make
./TurnBasedGame
```

### 2. Biên dịch bằng Visual Studio (MSVC)
1. Mở file solution `TurnBasedGame.slnx` hoặc `TurnBasedGame.vcxproj`.
2. Bấm `Ctrl + F5` để Build và Run dự án.

---

## 👥 Phân công Task Thành viên
- **Nguyễn Công Duẩn**: Task 0 (Setup repo, skeleton, workflow), Task 1-4 (Domain Models), Task 10-11 (BattleEngine).
- **Nguyễn Đình Dũng**: Task 5-6 (CharacterRoster & Persistence), Task 12 (DataFileManager), Task 15, 17, 19.
- **Đỗ Duệ Minh**: Task 7-9 (Team & TeamManager), Task 13-14 (Console UI & Menu), Task 18, 20.