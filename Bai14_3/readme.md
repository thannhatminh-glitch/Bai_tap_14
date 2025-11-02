
# Bài tập 3: STM32F103C8T6 - Gửi UART định kỳ và chuyển sang chế độ Standby

## 🎯 Mục tiêu
Viết chương trình cho **STM32F103C8T6** thực hiện các bước sau:
1. MCU thức dậy định kỳ nhờ **RTC** (Real Time Clock).
2. Gửi chuỗi dữ liệu qua **UART1** lên máy tính (ví dụ: `"Hello from STM32"`).
3. Sau khi gửi xong, chuyển MCU sang **chế độ Standby** để tiết kiệm năng lượng.
4. Đo và so sánh dòng tiêu thụ của vi điều khiển:
   - Khi **đang gửi UART**
   - Khi **ở chế độ Standby**

---

## ⚙️ Cấu hình phần cứng

| Thành phần | Kết nối | Mô tả |
|-------------|----------|--------|
| STM32F103C8T6 | - | Vi điều khiển chính |
| UART1 TX | PA9 | Gửi dữ liệu tới USB-UART hoặc máy tính |
| UART1 RX | PA10 | Nhận dữ liệu (tùy chọn) |
| RTC | LSE (32.768 kHz) | Tạo xung thời gian thực để đánh thức MCU |
| Nguồn đo | 3.3V | Dùng đồng hồ đo dòng tiêu thụ |

---

## 🧩 Thư viện cần thiết
- **CMSIS** và **Standard Peripheral Library** (`stm32f10x.h`, `stm32f10x_gpio.h`, `stm32f10x_usart.h`, `stm32f10x_rtc.h`, `stm32f10x_pwr.h`, `stm32f10x_bkp.h`, …)

---

## 📜 Mô tả chương trình

1. **Khởi tạo hệ thống và UART1**  
   - Baudrate: 9600 bps  
   - Truyền dữ liệu sang máy tính qua cổng COM.

2. **Cấu hình RTC**  
   - Sử dụng thạch anh ngoài LSE 32.768 kHz.  
   - Đặt bộ chia (prescaler) để RTC đếm giây.  
   - Dùng RTC để định kỳ đánh thức MCU sau mỗi 5 giây.

3. **Chu kỳ hoạt động**
   - Khi MCU khởi động hoặc thức dậy:
     - Gửi chuỗi `"Hello from STM32\r\n"` qua UART.
   - Sau khi gửi:
     - MCU vào **Standby Mode**.
   - Sau 5 giây, RTC báo thức → MCU **reset** và khởi động lại, lặp lại chu kỳ.

4. **Đo dòng tiêu thụ**
   - Dùng đồng hồ đo ampe kế ở dây **3.3V cấp cho VCC STM32**.
   - So sánh hai trường hợp:
     - Khi đang **gửi UART**
     - Khi **Standby**

---


## 📘 Ghi chú

* Khi thoát Standby, STM32F1 **reset lại toàn bộ hệ thống**, vì vậy chương trình bắt đầu từ đầu.
* Nếu muốn MCU **thức dậy mà không reset**, hãy dùng **chế độ STOP** thay vì STANDBY.
* Có thể quan sát dữ liệu UART bằng **PuTTY / RealTerm / TeraTerm** ở baudrate 9600.


```

