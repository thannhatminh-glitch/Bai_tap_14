
# STM32F103C8T6 - Sleep Mode & Wake-up by Button (WFI/WFE)

## 🧩 Giới thiệu
Dự án này minh họa cách **đưa STM32F103C8T6 vào chế độ Sleep** và **đánh thức (Wake-up)** bằng **ngắt ngoài (EXTI)** khi nhấn nút.  
Ngoài ra, chương trình cũng có phần **nháy LED cảnh báo trước khi vào Sleep**, và **chọn giữa hai cơ chế ngủ:**
- `__WFI()` – *Wait For Interrupt*  
- `__WFE()` – *Wait For Event*

Việc chọn cơ chế được điều khiển bằng macro:
```c
#define USE_WFI 1  // =1 dùng WFI, =0 dùng WFE
````

---

## ⚙️ Cấu hình phần cứng

| Thành phần                | Chân | Mô tả                                    |
| ------------------------- | ---- | ---------------------------------------- |
| LED (mặc định trên board) | PC13 | Dùng để hiển thị trạng thái hoạt động    |
| Nút nhấn (Button)         | PA0  | Dùng để tạo ngắt đánh thức vi điều khiển |

> ⚠️ Lưu ý: Nút PA0 cần **kéo lên (pull-up)** hoặc có **điện trở kéo ngoài** nếu không có trên board.

---

## 🧠 Mô tả hoạt động

1. Bật clock cho GPIOC.
2. Nháy LED 5 lần để báo hiệu chuẩn bị vào chế độ Sleep.
3. Tắt GPIOC và ngắt clock để tiết kiệm năng lượng.
4. Cấu hình nút PA0 làm **nguồn ngắt ngoài (EXTI0)** với sườn **Falling edge**.
5. Vi điều khiển **vào chế độ Sleep**:

   * Nếu `USE_WFI = 1` → Dùng lệnh `__WFI()`.
   * Nếu `USE_WFI = 0` → Dùng lệnh `__WFE()` với kỹ thuật "SEVONPEND" để đồng bộ.
6. Khi nhấn nút → xảy ra ngắt EXTI0 → CPU **thức dậy** khỏi chế độ Sleep.
7. Sau khi dậy, chương trình tiếp tục vòng lặp vô tận với `__NOP()`.

---

## 🧱 Cấu trúc mã nguồn

```
├── main.c
│   ├── delay_ms()        // Hàm delay cơ bản dùng vòng for và NOP
│   ├── Button_wakeup()   // Cấu hình PA0 làm ngắt đánh thức
│   ├── EXTI0_IRQHandler() // Hàm xử lý ngắt nút nhấn
│   └── main()            // Nháy LED, Sleep, và chờ đánh thức
```

---

## 🕹️ Cách biên dịch & nạp chương trình

* **IDE khuyến nghị**: Keil uVision / STM32CubeIDE / PlatformIO
* **Vi điều khiển**: STM32F103C8T6 (Blue Pill)
* **Clock hệ thống**: 72 MHz (mặc định HSE + PLL)
* Nạp code bằng ST-Link hoặc USB-TTL.

---

## 💡 Ghi chú kỹ thuật

* Hàm `delay_ms()` sử dụng vòng lặp rỗng, không phụ thuộc vào timer → sai số nhỏ.
* Khi vào Sleep:

  * **Tất cả clock ngoại vi tắt** để tiết kiệm năng lượng.
  * **Chỉ ngắt ngoài (EXTI0)** hoạt động để đánh thức CPU.
* Có thể thay `__WFI()` bằng `__WFE()` để so sánh mức tiêu thụ năng lượng.

---

## 📸 Mô phỏng minh họa (tùy chọn)

| Trạng thái       | LED PC13     | Ghi chú      |
| ---------------- | ------------ | ------------ |
| Đang hoạt động   | Nháy 5 lần   | Chuẩn bị ngủ |
| Đang Sleep       | Tắt          | Đợi ngắt PA0 |
| Sau khi nhấn nút | Sáng trở lại | CPU thức dậy |

---
