

## 🧠 BÀI TẬP EVENT GROUPS - FreeRTOS STM32F103C8T6



### ⚙️ Cấu hình phần cứng

| Thiết bị | Chân GPIO | Chức năng     | Ghi chú         |
| -------- | --------- | ------------- | --------------- |
| LED1     | PA2       | Output        | Nháy 300ms      |
| LED2     | PA3       | Output        | Nháy 500ms      |
| LED3     | PA4       | Output        | Nháy 700ms      |
| Nút nhấn | PA0       | Input Pull-up | Nhấn để đổi LED |

---


### 🧠 Nguyên lý hoạt động

1. **TaskButton** đọc nút nhấn PA0.

   * Mỗi lần nhấn → thay đổi “chế độ LED” (mode).
   * Gửi tín hiệu qua **Event Group** bằng các bit:

     * `BIT_TASK2` → LED1
     * `BIT_TASK3` → LED2
     * `BIT_TASK4` → LED3
   * Xóa các bit cũ để đảm bảo chỉ 1 LED hoạt động.

2. **3 Task LED (vTaskLED1, vTaskLED2, vTaskLED3)**

   * Mỗi task chờ bit riêng trong EventGroup.
   * Khi được kích hoạt → LED tương ứng nháy với chu kỳ riêng.
   * Khi không được kích hoạt → LED tắt hoàn toàn.

3. Hệ thống chỉ có **1 LED nháy tại 1 thời điểm**.

   * Lần nhấn 1 → LED1 nháy
   * Lần nhấn 2 → LED2 nháy
   * Lần nhấn 3 → LED3 nháy
   * Lần nhấn 4 → Tất cả tắt
   * Lặp lại chu trình.

---



### 🚀 Cách hoạt động

| Lần nhấn | LED hoạt động   | Ghi chú         |
| -------- | --------------- | --------------- |
| 1        | LED1 (PA2) nháy | Chu kỳ 300ms    |
| 2        | LED2 (PA3) nháy | Chu kỳ 500ms    |
| 3        | LED3 (PA4) nháy | Chu kỳ 700ms    |
| 4        | Tắt tất cả      | Quay lại vòng 1 |

---

### 🧰 Kiến thức áp dụng

* Cơ chế **Event Groups** trong FreeRTOS.
* **Task Synchronization** (đồng bộ tác vụ).
* **Chống dội phím (debounce)** bằng `vTaskDelay()`.
* Lập trình **đa nhiệm RTOS** trên **STM32F103C8T6**.
* **Điều phối tác vụ** bằng bit event (kích hoạt đơn).

---

