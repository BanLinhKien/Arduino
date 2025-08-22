# Bắt Đầu Với Arduino UNO R3

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_8.png" alt="Arduino" width="400" style="display: block; margin: 0 auto;" />

Bất kỳ một chương trình học nào cũng cần nên bắt đầu một cách từ từ. Bởi vì thời điểm này chúng ta đều mới bắt đầu, nhiều khái niệm, kiến thức về lĩnh vực này gần như không có nhiều. Ở bài này giúp các bạn có thể nắm được các kiến thức cơ bản về điện tử, làm sao để biên dịch, nạp được chương trình trong **Arduino**, cũng như nắm được một số kiến thức về kiến trúc chương trình của **Arduino**. Nội dung sẽ tìm hiểu ở phần này như sau:
- Giới thiệu các khái niệm, linh kiện điện tử cơ bản.
- Blink LED.
- Nút nhấn và các ứng dụng.
- Sử dụng chức năng PWM trong Arduino.
- Đọc dữ liệu Analog từ cảm biến.

## 1. Giới thiệu một số khái niệm và linh kiện điện tử cơ bản

Trong phần này chúng ta sẽ tìm hiểu về 1 số khái niệm cũng như 1 số linh kiện điện tử cơ bản. Các kiến thức này cũng đã được trình bày chi tiết ở chương trình vật lí bậc phổ thông. Mỗi khái niệm hay linh kiện sẽ có những video giúp chúng ta dễ hiểu hơn những khái niệm cũng như cách hoạt động của các linh kiện điện tử thông dụng.

## Điện áp, dòng điện và điện trở

### Điện áp
**Điện áp** hay còn gọi là hiệu điện thế (tiếng Anh: voltage) là sự chênh lệch về điện áp giữa 2 điểm, nó là công thực hiện được để di chuyển một hạt điện tích trong trường tĩnh điện từ điểm này đến điểm kia. Hiệu điện thế có thể đại diện cho nguồn năng lượng (lực điện), hoặc sự mất đi, sử dụng, hoặc năng lượng lưu trữ (giảm thế).

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/3_1.png" alt="Arduino" width="400" style="display: block; margin: 0 auto;" />


### Dòng điện
**Dòng điện** (tiếng Anh: electric current) là dòng chuyển dịch có hướng của các hạt mang điện. Trong mạch điện, các hạt mang điện phần lớn là các electron chuyển động bên trong dây dẫn. Kim loại là chất dẫn diện phổ biến nhất, kim loại có hạt nhân mang điện tích dương không thể di
chhuyển, chỉ có các electron tích điện âm có khả năng di chuyển tự do trong vùng dẫn, do đó, trong kim loại các electron là các hạt mang điện.

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/3_2.png" alt="Arduino" width="400" style="display: block; margin: 0 auto;" />

Chiều dòng điện được quy ước là chiều đi từ cực dương qua dây dẫn và các thiết bị điện đến cực âm của nguồn. Do dòng điện được qui ước là dòng chuyển dời có hướng của các điện tích dương, chính vì thế, trong mạch điện với dây dẫn kim loại, các electron tích điện âm dịch chuyển ngược chiều với chiều của dòng điện trong dây dẫn.

Sự chuyển dịch có hướng của các điện tích sinh ra do tác động của điện trường gây ra bởi hiệu điện thế. Do đó, có thể hiểu là điện áp sinh ra dòng điện trong một mạch điện, hay nói đơn gỉan là "điện áp có trước dòng điện" trong mạch điện.

Chúng ta có thể xem giải thích về dòng điện và điện áp theo cách dễ hiểu hơn với một số video:
- What is Voltage: www.youtube.com/watch?v=V1ulri4s_E8
- Electricity and Circuits: www.youtube.com/watch?v=D2mõnVkCkX

### Điện trở

**Điện trở** (tiếng Anh: electric resistance) là một đại lượng đặc trưng cho khả năng cản trở dòng điện của một vật. Đơn vị của điện trở là **Ω**. Khái niệm điện trở của vật xuất phát từ định luật Ohm. 

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/3_3" alt="Arduino" width="300" style="display: block; margin: 0 auto;" />

Điện trở gồm 2 tiếp điểm kết nối, thường được dùng để hạn chế cường độ dòng điện chạy trong mạch, điều chỉnh mức độ tín hiệu, dùng để chia điện áp, kích hoạt các linh kiện điện tử chủ động nhưtransistor và có trong rất nhiều ứng dụng khác.

### Định luật Ohm (Ohm’s law)

**Phát biểu định luật:**
Cường độ dòng điện chạy qua dây dẫn tỉ lệ thuận với hiệu điện thế đặt vào 2 đầu dây dẫn và tỉ lệ nghịch với điện trở của dây dẫn.
Công thức của định luật Ohm: \( I = \dfrac{V}{R} \)
Trong đó:
- V: Ký hiệu của điện áp (Voltage).
- I: Ký hiệu của dòng điện (Current).
- R: Ký hiệu của điện trở (Resistor).

### Tụ điện

**Khái niệm**
**Tụ điện** là linh kiện điện tử gồm 2 vật dẫn đặt gần nhau. Mỗi vật dẫn đó gọi là một bản của tụ điện. Khoảng không gian giữa hai bản có thể là chân không hay bị chiếm bởi một chất điện môi nào đó.

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/3_4.png" alt="Arduino" width="300" style="display: block; margin: 0 auto;" />

Tụ điện là một linh kiện được sử dụng rất phổ biến và gần như không thể thiếu trong các mạch điện tử, mỗi tụ điện đều có một công dụng nhất định như lọc nhiễu cho mạch, tạo dao động, truyền dẫn tín hiệu,...

Các thông số đặc trưng:
- Điện dung: Đại diện cho khả năng tích điện của tụ. Đơn vị là Fara (F).
- Điện áp làm việc: Đó là giá trị điện áp cao nhất mà tụ điện có thể chịu được, thông thường giá trị này sẽ được ghi trên thân của tụ điện (nếu tụ đủ lớn). Nếu giá trị điện áp trên tụ lớn hơn giá trị điện áp làm việc thì lớp điện môi bên trong tụ điện sẽ bị đánh thủng và gây ra sự chập tụ, nổ tụ. Hiện tượng này khá nguy hiểm nên chúng ta cần cẩn thận khi chọn tụ điện cho mạch điện của mình.
- Nhiệt độ làm việc: Đó là nhiệt độ ở vùng đặt tụ điện trong mạch điện. Tụ điện nên được chọn với nhiệt độ làm việc cao nhất của nơi chúng ta đặt tụ điện phải cao hơn nhiệt độ này.

Để hiểu rõ hơn về cách hoạt động của tụ điện, có thể tham khảo tại đây: www.youtube.com/watch?v=5hFC9ugTGLs

### Cuộn cảm

**Khái niệm**
**Cuộn cảm** (tiếng Anh là coil hay inductor) là cuộn dây bao gồm nhiều vòng dây dẫn điện quấn quanh một lõi vật liệu từ. Dựa vào hiện tượng cảm ứng điện từ và hiện tượng từ hóa vật liệu từ mà người ta sử dụng cuộn cảm cho các mục đích khác nhau như làm phần ứng (stator) trong các máy phát điện xoay chiều, lọc nhiễu trong các mạch điện tử, tạo ra các nam châm điện, các công tắc điện tử, ...

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/3_5.webp" alt="Arduino" width="350" style="display: block; margin: 0 auto;" />

Mỗi cuộn cảm có một độ tự cảm (hay hệ số tự cảm hoặc từ dung), kí hiệu là **L**, đo bằng đơn vị Henry (**H**) đặc trưng cho khả năng sinh suất điện động cảm ứng và tích lũy năng lượng điện từ.

Để hiểu rõ hơn về cách hoạt động và chức năng của cuộn cảm, chúng ta có thể tham khảo tại đây: www.youtube.com/watch?v=NgwXkUt3XxQ

---