# import tkinter as tk
# from tkinter import ttk
# from tkintermapview import TkinterMapView
#
# class MapApp(tk.Frame):
#     def __init__(self, parent):
#         super().__init__(parent)
#         self.parent = parent
#         self.pack()
#
#         # Tạo widget bản đồ
#         self.map_widget = TkinterMapView(self, width=800, height=600)
#         self.map_widget.pack()
#
#         # Tạo danh sách lưu các markers
#         self.markers = []
#
#         # Khởi tạo bản đồ mặc định
#         self.map_widget.set_position(21.0285, 105.8542)  # Vị trí mặc định là Hà Nội
#         self.add_marker(21.0285, 105.8542, "Hà Nội")  # Chấm đỏ ở Hà Nội
#
#     def add_marker(self, lat, lon, text):
#         # Thêm marker mới vào bản đồ
#         new_marker = self.map_widget.set_marker(lat, lon, text=text, marker_color_outside="blue", marker_color_circle="red")
#         self.markers.append(new_marker)
#
#     def update_map(self, location):
#         # Xóa tất cả các marker hiện tại
#         for marker in self.markers:
#             marker.delete()
#
#         if location == "database1.db":
#             self.map_widget.set_position(21.0285, 105.8542)  # Vị trí Hà Nội
#             self.add_marker(21.0285, 105.8542, "Hà Nội")  # Chấm đỏ ở Hà Nội
#         elif location == "database2.db":
#             self.map_widget.set_position(10.8231, 106.6297)  # Vị trí Hồ Chí Minh
#             self.add_marker(10.8231, 106.6297, "Hồ Chí Minh")  # Chấm đỏ ở Hồ Chí Minh
#         elif location == "database3.db":
#             self.map_widget.set_position(16.0544, 108.2022)  # Vị trí Đà Nẵng
#             self.add_marker(16.0544, 108.2022, "Đà Nẵng")  # Chấm đỏ ở Đà Nẵng
#
# class MainApp:
#     def __init__(self, root):
#         self.root = root
#         self.root.title("Ứng dụng Bản đồ")
#
#         # Tạo MapApp và nhúng vào cửa sổ chính
#         self.map_app = MapApp(self.root)
#         self.map_app.pack()
#
#         # Tạo khung cho các radiobutton
#         self.frame = ttk.Frame(self.root)
#         self.frame.pack()
#
#         # Tạo các radiobutton
#         self.radiobutton_hanoi = ttk.Radiobutton(self.frame, text="Hà Nội", value="database1.db", command=lambda: self.update_map("database1.db"))
#         self.radiobutton_hcm = ttk.Radiobutton(self.frame, text="Hồ Chí Minh", value="database2.db", command=lambda: self.update_map("database2.db"))
#         self.radiobutton_danang = ttk.Radiobutton(self.frame, text="Đà Nẵng", value="database3.db", command=lambda: self.update_map("database3.db"))
#
#         # Đặt vị trí các radiobutton
#         self.radiobutton_hanoi.grid(row=0, column=0)
#         self.radiobutton_hcm.grid(row=0, column=1)
#         self.radiobutton_danang.grid(row=0, column=2)
#
#     def update_map(self, location):
#         # Gọi phương thức update_map của MapApp để thay đổi bản đồ
#         self.map_app.update_map(location)
#
# # Chạy chương trình
# if __name__ == "__main__":
#     root = tk.Tk()
#     app = MainApp(root)
#     root.mainloop()


import tkinter as tk
from tkinter import PhotoImage


class ImageRadioApp:
    def __init__(self, root):
        self.root = root

        # Khung hiển thị ảnh
        self.image_label = tk.Label(root)
        self.image_label.pack(pady=20)

        # Label hiển thị địa điểm
        self.location_label = tk.Label(root, text="Địa điểm: ")
        self.location_label.pack(pady=10)

        # Tải các hình ảnh
        self.image1 = PhotoImage(file="image1.png")  # Thay bằng đường dẫn tới ảnh đầu tiên
        self.image2 = PhotoImage(file="image2.png")  # Thay bằng đường dẫn tới ảnh thứ hai
        self.image3 = PhotoImage(file="image3.png")  # Thay bằng đường dẫn tới ảnh thứ ba

        # Danh sách địa điểm tương ứng với từng ảnh
        self.locations = {
            "database1.db": "Hà Nội",
            "database2.db": "Hồ Chí Minh",
            "database3.db": "Đà Nẵng"
        }

        # Hiển thị ảnh và địa điểm đầu tiên mặc định
        self.image_label.config(image=self.image1)
        self.location_label.config(text=f"Địa điểm: {self.locations['database1.db']}")

    def update_image(self, selected_value):
        if selected_value == "database1.db":
            self.image_label.config(image=self.image1)
        elif selected_value == "database2.db":
            self.image_label.config(image=self.image2)
        elif selected_value == "database3.db":
            self.image_label.config(image=self.image3)

        # Cập nhật địa điểm
        self.location_label.config(text=f"Địa điểm: {self.locations[selected_value]}")


if __name__ == "__main__":
    root = tk.Tk()
    root.title("Hiển thị ảnh với RadioButton")
    root.geometry("600x600")

    # Tạo ứng dụng chính
    app = ImageRadioApp(root)

    # Biến để lưu giá trị của RadioButton
    selected_option = tk.StringVar(value='database1.db')


    # Hàm cập nhật ảnh dựa trên RadioButton
    def on_radio_change():
        app.update_image(selected_option.get())


    # Tạo các RadioButton
    radio1 = tk.Radiobutton(root, text="Ảnh 1", variable=selected_option, value="database1.db", command=on_radio_change)
    radio1.pack()

    radio2 = tk.Radiobutton(root, text="Ảnh 2", variable=selected_option, value="database2.db", command=on_radio_change)
    radio2.pack()

    radio3 = tk.Radiobutton(root, text="Ảnh 3", variable=selected_option, value="database3.db", command=on_radio_change)
    radio3.pack()

    # Hiển thị giao diện
    root.mainloop()
