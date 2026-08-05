# TURN-BASED ADVENTURE GAME MOCK PROJECT

Chương trình console mô phỏng trận đấu theo lượt giữa hai đội nhân vật (C++11).

## ⚙️ Môi trường & Công cụ Phát triển (Compiler & IDE Specs)
Để đảm bảo nhất quán cho tất cả thành viên trong nhóm khi build và chạy code:
- **Chuẩn C++**: ISO C++17 (`-std=c++17` đối với GCC / `/std:c++17` đối với MSVC).
- **Trình biên dịch (Compiler)**:
  - GCC / MinGW g++ 7.0 trở lên (Linux / Windows MinGW).
  - MSVC v143 (Microsoft Visual C++ đi kèm Visual Studio 2022).
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

## 🏛️ Kiến trúc Hệ thống

Hệ thống theo mô hình **3-Layer Architecture**. Sơ đồ bên dưới render được trực tiếp trên GitHub (qua Mermaid) và trong VS Code (qua extension PlantUML).

```mermaid
graph TD
    subgraph PRESENTATION["🖥️ Presentation Layer"]
        Menu["Menu\n(Console UI / GameApp)\n─────────────────\n• Nhận input từ người dùng\n• Hiển thị output ra console\n• Điều phối các use case\n• Không chứa business logic"]
    end

    subgraph APPLICATION["⚙️ Application Layer"]
        Roster["CharacterRoster\n─────────────────\n• Sở hữu collection Character*\n• Validation: ID trùng, tên rỗng\n• Tìm theo ID / tên (case-insensitive)\n• Xóa nhân vật theo ID"]

        TeamMgr["TeamManager\n─────────────────\n• Lưu Character ID, không sở hữu Character\n• Tạo / đổi tên / xóa / liệt kê Team\n• Giữ đúng thứ tự nhân vật đã thêm\n• Validate ID trong Roster trước khi thêm"]

        Battle["BattleEngine\n─────────────────\n• Lượt luân phiên A/B\n• READY→IN_PROGRESS→FINISHED\n• performAction() qua Character*\n• Không kết nối Persistence"]
    end

    subgraph PERSISTENCE["💾 Persistence Layer"]
        DFM["DataFileManager\n─────────────────\n• Đọc / ghi characters.txt\n• Đọc / ghi teams.txt\n• Bỏ qua dòng sai, báo rõ lý do\n• Báo lỗi nếu không mở/ghi được file"]
    end

    subgraph FILES["📂 File System"]
        CharFile[("characters.txt")]
        TeamFile[("teams.txt")]
    end

    %% Presentation → Application
    Menu -->|"addWarrior/addMage/updateCharacter\nremoveById/findById/displayAll\nload startup / save Exit"| Roster
    Menu -->|"createTeam/renameTeam/deleteTeam\naddCharacterToTeam/removeCharacterFromTeam\nremoveCharacterFromAllTeams\nload startup / save Exit"| TeamMgr
    Menu -->|"selectTeams/startBattle\nperformCurrentAction\ngetStatus/getCurrentActor"| Battle

    %% Application → Application
    TeamMgr -.->|"exists id / findById\nread-only, validate trước khi thêm"| Roster
    Battle  -.->|"findById → Character*\nnon-owning, dùng performAction"| Roster

    %% Application → Persistence
    Roster  -->|"loadCharacters / saveCharacters"| DFM
    TeamMgr -->|"loadTeams / saveTeams"| DFM

    %% Persistence → Files
    DFM -->|"đọc / ghi"| CharFile
    DFM -->|"đọc / ghi"| TeamFile

    %% Styles
    style PRESENTATION fill:#AED6F1,stroke:#2E86C1,color:#000
    style APPLICATION  fill:#A9DFBF,stroke:#1E8449,color:#000
    style PERSISTENCE  fill:#F9E79F,stroke:#D4AC0D,color:#000
    style FILES        fill:#FAD7A0,stroke:#CA6F1E,color:#000
```

### Tóm tắt quan hệ giữa các layer

| Từ | Đến | Mô tả |
|---|---|---|
| Menu | CharacterRoster | CRUD nhân vật, load/save |
| Menu | TeamManager | CRUD team, load/save, cascade delete |
| Menu | BattleEngine | Điều khiển trận đấu, đọc trạng thái |
| TeamManager | CharacterRoster | Kiểm tra ID tồn tại trước khi thêm vào Team |
| BattleEngine | CharacterRoster | Lấy `Character*` để gọi `performAction()` |
| CharacterRoster | DataFileManager | Nạp / lưu characters.txt |
| TeamManager | DataFileManager | Nạp / lưu teams.txt |
| DataFileManager | File system | Đọc / ghi `.txt` |

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