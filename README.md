# TURN-BASED ADVENTURE GAME MOCK PROJECT

Chương trình console mô phỏng trận đấu theo lượt giữa hai đội nhân vật (C++11).

## 📁 Cấu trúc Thư mục
```text
TurnBasedGame/
├── include/       # Header files (.h)
├── src/           # Implementation files (.cpp)
├── data/          # File dữ liệu mẫu (characters.txt, teams.txt)
├── tests/         # Test cases & unit tests
├── Makefile       # Biên dịch bằng GCC / MinGW g++
├── .gitignore     # Loại bỏ file build rác
├── README.md      # Tài liệu hướng dẫn
└── main.cpp       # Điểm khởi chạy ứng dụng
```

## 🛠️ Hướng dẫn Biên dịch và Chạy

### 1. Biên dịch bằng GCC (g++ / Makefile)
```bash
make
./TurnBasedGame
```

### 2. Biên dịch bằng Visual Studio (MSVC)
1. Mở file solution `TurnBasedGame.slnx` hoặc `TurnBasedGame.vcxproj`.
2. Bấm `Ctrl + F5` để Build và Run.

## 👥 Phân công Task thành viên
- **Nguyễn Công Duẩn**: Task 1 (Skeleton & Setup), Task 2-4 (Domain Models), Task 10-11 (BattleEngine).
- **Nguyễn Đình Dũng**: Task 5-6 (CharacterRoster & Data Persistence), Task 12 (DataFileManager).
- **Đỗ Duệ Minh**: Task 7-9 (Team & TeamManager), Task 13-14 (Console UI & Menu).