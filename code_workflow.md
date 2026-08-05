# Workflow mỗi khi code 1 tính năng mới

## 1. Pull latest code

Trước khi bắt đầu task, pull code mới nhất từ branch main:

```bash
git checkout main
git pull origin main
```

---

## 2. Create branch

Tạo branch riêng cho feature:

```bash
git checkout -b feature/<feature-name>
```

Ví dụ:

```bash
git checkout -b feature/add-character
```

---

## 3. Code + Unit Test

Thực hiện:

- Code feature được giao.
- Viết Unit Test cho logic đã implement.
- Kiểm tra code build thành công.
- Đảm bảo các test case pass.

---

## 4. Commit changes

Kiểm tra thay đổi:

```bash
git status
```

Add file:

```bash
git add .
```

Commit: (commit message phải dễ hiểu, ngắn gọn)

```bash
git commit -m "Hòan thành <feature-name>"
```

Ví dụ:

```bash
git commit -m "Implement character validation"
```

---

## 5. Push branch

Push code lên GitHub:

```bash
git push origin feature/<feature-name>
```

---

## 6. Create Pull Request (PR)

Trên GitHub:

- Chọn branch vừa push.
- Tạo Pull Request vào branch `main`.
- Mô tả:
  - Feature đã làm.
  - File thay đổi.
  - Test đã chạy.

---

## 7. Code Review

Reviewer kiểm tra hoặc có thể tag @copilot AI để kiểm tra trước khi merge vào branch main để tránh lỗi.

- Code đúng yêu cầu.
- Không có bug.
- Unit Test đầy đủ.
- Không ảnh hưởng module khác.

Nếu có feedback:
- Sửa code.
- Commit bổ sung.
- Push lại branch.

---

## 8. Merge PR

Sau khi PR được approve:

- Merge Pull Request vào `main`.
- Xóa branch feature nếu không còn sử dụng.

---

## 9. Update local code

Sau khi merge:

```bash
git checkout main
git pull origin main
```

Sẵn sàng nhận task tiếp theo.
