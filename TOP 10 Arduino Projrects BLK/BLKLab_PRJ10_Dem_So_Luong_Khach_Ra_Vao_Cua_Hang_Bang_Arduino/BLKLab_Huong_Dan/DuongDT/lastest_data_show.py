# import customtkinter as ctk
# import sqlite3
# import json
# import os
# from datetime import datetime
#
# # Đường dẫn đến tệp ngưỡng
# THRESHOLDS_FILE = 'thresholds.json'
#
#
# # Hàm tải ngưỡng từ tệp JSON
# def load_thresholds():
#     if not os.path.exists(THRESHOLDS_FILE):
#         # Nếu tệp không tồn tại, tạo tệp với ngưỡng mặc định
#         default_thresholds = {
#             "temp": {"min": 15.0, "max": 30.0},
#             "humi": {"min": 30.0, "max": 70.0},
#             "co": {"max": 10.0},
#             "pm25": {"max": 35.0},
#             "nh3": {"max": 25.0},
#             "airpresser": {"max": 1013.0}
#         }
#         with open(THRESHOLDS_FILE, 'w') as f:
#             json.dump(default_thresholds, f, indent=4)
#         return default_thresholds
#     else:
#         with open(THRESHOLDS_FILE, 'r') as f:
#             return json.load(f)
#
#
# # Hàm lưu ngưỡng vào tệp JSON
# def save_thresholds(thresholds):
#     with open(THRESHOLDS_FILE, 'w') as f:
#         json.dump(thresholds, f, indent=4)
#
#
# # Kết nối tới cơ sở dữ liệu SQLite và lấy dữ liệu mới nhất
# def get_latest_measurement():
#     try:
#         conn = sqlite3.connect('database.db')
#         cursor = conn.cursor()
#         # Truy vấn để lấy dòng mới nhất dựa trên rowid
#         cursor.execute("""
#             SELECT date_save, time_save, temp, humi, co, pm25, nh3, airpresser
#             FROM measurements
#             ORDER BY rowid DESC
#             LIMIT 1
#         """)
#         row = cursor.fetchone()
#         conn.close()
#         if row:
#             return {
#                 "date_save": row[0],
#                 "time_save": row[1],
#                 "temp": row[2],
#                 "humi": row[3],
#                 "co": row[4],
#                 "pm25": row[5],
#                 "nh3": row[6],
#                 "airpresser": row[7]
#             }
#         else:
#             return {}
#     except sqlite3.Error as e:
#         print(f"Lỗi kết nối cơ sở dữ liệu: {e}")
#         return {}
#
#
# # Lớp cửa sổ cài đặt ngưỡng
# class SettingsWindow(ctk.CTkToplevel):
#     def __init__(self, parent):
#         super().__init__(parent)
#         self.title("Cài đặt ngưỡng")
#         self.geometry("400x400")
#         self.resizable(False, False)
#
#         self.parent = parent
#         self.thresholds = parent.thresholds  # Truy cập ngưỡng hiện tại
#
#         # Tạo khung chính
#         self.main_frame = ctk.CTkFrame(self, corner_radius=10)
#         self.main_frame.pack(pady=20, padx=20, fill="both", expand=True)
#
#         # Tạo biểu mẫu cài đặt ngưỡng cho từng thông số
#         self.entries = {}
#         row = 0
#         for sensor in parent.sensors:
#             key = sensor["key"]
#             name = sensor["name"]
#
#             label = ctk.CTkLabel(self.main_frame, text=name, font=("Helvetica", 12, "bold"))
#             label.grid(row=row, column=0, pady=10, padx=10, sticky="w")
#
#             if key in ["temp", "humi"]:
#                 # Thông số có cả ngưỡng dưới và trên
#                 min_label = ctk.CTkLabel(self.main_frame, text="Min:", font=("Helvetica", 10))
#                 min_label.grid(row=row, column=1, padx=5, sticky="e")
#                 min_entry = ctk.CTkEntry(self.main_frame, width=80)
#                 min_entry.grid(row=row, column=2, padx=5, sticky="w")
#                 min_entry.insert(0, str(self.thresholds[key].get("min", "")))
#
#                 max_label = ctk.CTkLabel(self.main_frame, text="Max:", font=("Helvetica", 10))
#                 max_label.grid(row=row, column=3, padx=5, sticky="e")
#                 max_entry = ctk.CTkEntry(self.main_frame, width=80)
#                 max_entry.grid(row=row, column=4, padx=5, sticky="w")
#                 max_entry.insert(0, str(self.thresholds[key].get("max", "")))
#
#                 self.entries[key] = {"min": min_entry, "max": max_entry}
#             else:
#                 # Thông số chỉ có ngưỡng trên
#                 max_label = ctk.CTkLabel(self.main_frame, text="Max:", font=("Helvetica", 10))
#                 max_label.grid(row=row, column=1, padx=5, sticky="e")
#                 max_entry = ctk.CTkEntry(self.main_frame, width=80)
#                 max_entry.grid(row=row, column=2, padx=5, sticky="w")
#                 max_entry.insert(0, str(self.thresholds[key].get("max", "")))
#
#                 self.entries[key] = {"max": max_entry}
#
#             row += 1
#
#         # Nút lưu cài đặt
#         self.save_button = ctk.CTkButton(
#             self.main_frame,
#             text="Lưu cài đặt",
#             command=self.save_settings
#         )
#         self.save_button.grid(row=row, column=0, columnspan=5, pady=20)
#
#     def save_settings(self):
#         # Lấy giá trị từ các ô nhập và cập nhật ngưỡng
#         for key, entry in self.entries.items():
#             if key in ["temp", "humi"]:
#                 min_val = entry["min"].get()
#                 max_val = entry["max"].get()
#                 try:
#                     min_val = float(min_val)
#                 except ValueError:
#                     min_val = None
#                 try:
#                     max_val = float(max_val)
#                 except ValueError:
#                     max_val = None
#                 self.thresholds[key]["min"] = min_val
#                 self.thresholds[key]["max"] = max_val
#             else:
#                 max_val = entry["max"].get()
#                 try:
#                     max_val = float(max_val)
#                 except ValueError:
#                     max_val = None
#                 self.thresholds[key]["max"] = max_val
#
#         # Lưu ngưỡng vào tệp JSON
#         save_thresholds(self.thresholds)
#
#         # Cập nhật ngưỡng trong ứng dụng chính
#         self.parent.thresholds = self.thresholds
#
#         # Đóng cửa sổ cài đặt
#         self.destroy()
#
#
# # Lớp SensorApp kế thừa từ CTkFrame
# class SensorApp(ctk.CTkFrame):
#     def __init__(self, parent, *args, **kwargs):
#         super().__init__(parent, *args, **kwargs)
#
#         # Tải ngưỡng
#         self.thresholds = load_thresholds()
#
#         # Biến kiểm soát nhấp nháy
#         self.blink_flags = {sensor: False for sensor in self.thresholds}
#         self.blink_states = {sensor: False for sensor in self.thresholds}
#
#         # Tạo khung để hiển thị ngày và giờ
#         self.datetime_frame = ctk.CTkFrame(self, corner_radius=10)
#         self.datetime_frame.pack(pady=10, padx=20, fill="x")
#
#         self.date_label = ctk.CTkLabel(
#             self.datetime_frame,
#             text="Ngày: N/A",
#             font=("Helvetica", 14)
#         )
#         self.date_label.pack(side="left", padx=10)
#
#         self.time_label = ctk.CTkLabel(
#             self.datetime_frame,
#             text="Giờ: N/A",
#             font=("Helvetica", 14)
#         )
#         self.time_label.pack(side="right", padx=10)
#
#         # Tạo khung chính với lưới 2x3
#         self.main_frame = ctk.CTkFrame(self, corner_radius=10)
#         self.main_frame.pack(pady=20, padx=20, fill="both", expand=True)
#
#         self.main_frame.grid_columnconfigure((0, 1, 2), weight=1)
#         self.main_frame.grid_rowconfigure((0, 1), weight=1)
#
#         # Danh sách các thông số và màu sắc tương ứng (chỉ bao gồm 6 cảm biến)
#         self.sensors = [
#             {"key": "temp", "name": "Nhiệt độ (°C)", "color": "#90EE90"},  # Xanh lá nhạt
#             {"key": "humi", "name": "Độ ẩm (%)", "color": "#FFA07A"},  # Cam nhạt
#             {"key": "co", "name": "CO (ppm)", "color": "#E6E6FA"},  # Tím nhạt
#             {"key": "pm25", "name": "PM2.5 (µg/m³)", "color": "#FFD700"},  # Vàng
#             {"key": "nh3", "name": "NH3 (ppm)", "color": "#87CEFA"},  # Xanh dương nhạt
#             {"key": "airpresser", "name": "Áp suất không khí (hPa)", "color": "#98FB98"}  # Xanh lá mát
#         ]
#
#         self.labels = {}
#         self.boxes = {}
#
#         for index, sensor in enumerate(self.sensors):
#             row = index // 3  # 0 hoặc 1
#             col = index % 3  # 0,1,2
#
#             # Tạo khung cho mỗi ô
#             box = ctk.CTkFrame(
#                 self.main_frame,
#                 corner_radius=10,
#                 fg_color=sensor["color"],
#                 height=100
#             )
#             box.grid(row=row, column=col, padx=10, pady=10, sticky="nsew")
#             self.boxes[sensor["key"]] = box
#
#             # Tạo label tên thông số
#             label_name = ctk.CTkLabel(
#                 box,
#                 text=sensor["name"],
#                 font=("Helvetica", 14, "bold")
#             )
#             label_name.pack(pady=(10, 5))
#
#             # Tạo label giá trị thông số
#             label_value = ctk.CTkLabel(
#                 box,
#                 text="N/A",
#                 font=("Helvetica", 16)
#             )
#             label_value.pack(pady=(0, 10))
#
#             self.labels[sensor["key"]] = label_value
#
#         # Nút cập nhật thủ công
#         self.update_button = ctk.CTkButton(
#             self,
#             text="Cập nhật ngay",
#             command=self.update_data
#         )
#         self.update_button.pack(pady=10)
#
#         # Nút mở cửa sổ cài đặt ngưỡng
#         self.settings_button = ctk.CTkButton(
#             self,
#             text="Cài đặt ngưỡng",
#             command=self.open_settings
#         )
#         self.settings_button.pack(pady=10)
#
#         # Cập nhật dữ liệu tự động mỗi 5 giây
#         self.update_data()
#         self.after(5000, self.auto_update)
#
#     def open_settings(self):
#         SettingsWindow(self)
#
#     def update_data(self):
#         data = get_latest_measurement()
#         if data:
#             # Cập nhật ngày và giờ
#             self.date_label.configure(text=f"Ngày: {data.get('date_save', 'N/A')}")
#             self.time_label.configure(text=f"Giờ: {data.get('time_save', 'N/A')}")
#
#             # Cập nhật các thông số cảm biến
#             for sensor in self.sensors:
#                 key = sensor["key"]
#                 value = data.get(key, "N/A")
#                 self.labels[key].configure(text=str(value))
#
#                 # Kiểm tra ngưỡng
#                 threshold = self.thresholds.get(key, {})
#                 danger = False
#
#                 if key in ["temp", "humi"]:
#                     min_val = threshold.get("min", None)
#                     max_val = threshold.get("max", None)
#                     if min_val is not None and value < min_val:
#                         danger = True
#                     if max_val is not None and value > max_val:
#                         danger = True
#                 else:
#                     max_val = threshold.get("max", None)
#                     if max_val is not None and value > max_val:
#                         danger = True
#
#                 if danger:
#                     # Bắt đầu nhấp nháy
#                     if not self.blink_flags[key]:
#                         self.blink_flags[key] = True
#                         self.blink_box(key)
#                 else:
#                     # Kết thúc nhấp nháy và đặt màu nền bình thường
#                     self.blink_flags[key] = False
#                     self.boxes[key].configure(fg_color=sensor["color"])
#         else:
#             # Nếu không có dữ liệu, hiển thị "N/A"
#             self.date_label.configure(text="Ngày: N/A")
#             self.time_label.configure(text="Giờ: N/A")
#             for sensor in self.sensors:
#                 key = sensor["key"]
#                 self.labels[key].configure(text="N/A")
#                 self.boxes[key].configure(fg_color=sensor["color"])
#
#     def auto_update(self):
#         self.update_data()
#         self.after(5000, self.auto_update)  # Lặp lại sau mỗi 5000ms (5 giây)
#
#     def blink_box(self, key):
#         if not self.blink_flags[key]:
#             # Nếu nhấp nháy đã bị dừng, đảm bảo màu nền bình thường
#             self.boxes[key].configure(fg_color=self.get_sensor_color(key))
#             return
#
#         # Đảo trạng thái nhấp nháy
#         self.blink_states[key] = not self.blink_states[key]
#         if self.blink_states[key]:
#             # Đổi màu nền thành đỏ để cảnh báo
#             self.boxes[key].configure(fg_color="#FF6347")  # Tomato (đỏ cam)
#         else:
#             # Đổi màu nền về màu gốc
#             self.boxes[key].configure(fg_color=self.get_sensor_color(key))
#
#         # Gọi lại hàm này sau 500ms để tạo chu kỳ 1s
#         self.after(500, lambda: self.blink_box(key))
#
#     def get_sensor_color(self, key):
#         for sensor in self.sensors:
#             if sensor["key"] == key:
#                 return sensor["color"]
#         return "#FFFFFF"  # Màu trắng mặc định
#
#
# # Ví dụ về cách nhúng SensorApp vào một ứng dụng khác
# class MainApplication(ctk.CTk):
#     def __init__(self):
#         super().__init__()
#
#         self.title("Ứng dụng chính")
#         self.geometry("1000x700")
#         self.resizable(False, False)
#
#         # Tạo một khung chính
#         self.main_frame = ctk.CTkFrame(self, corner_radius=10)
#         self.main_frame.pack(pady=20, padx=20, fill="both", expand=True)
#
#         # Nhúng SensorApp vào khung chính
#         self.sensor_app = SensorApp(self.main_frame)
#         self.sensor_app.pack(fill="both", expand=True)
#
#         # Thêm các thành phần khác cho ứng dụng chính
#         # Ví dụ: menu, sidebar, các tab khác, v.v.
#         # Bạn có thể tùy chỉnh theo nhu cầu của mình
#
#
# # Chạy ứng dụng
# if __name__ == "__main__":
#     app = MainApplication()
#     app.mainloop()



import customtkinter as ctk
import sqlite3
import json
import os
from datetime import datetime
import tkinter.messagebox as messagebox

# Cấu hình giao diện CustomTkinter
ctk.set_appearance_mode("System")  # Modes: system (default), dark, light
ctk.set_default_color_theme("blue")  # Themes: blue (default), dark, green

# Đường dẫn đến tệp ngưỡng
THRESHOLDS_FILE = 'thresholds.json'


# Hàm tải ngưỡng từ tệp JSON
def load_thresholds():
    if not os.path.exists(THRESHOLDS_FILE):
        # Nếu tệp không tồn tại, tạo tệp với ngưỡng mặc định
        default_thresholds = {
            "temp": {"min": 15.0, "max": 30.0},
            "humi": {"min": 30.0, "max": 70.0},
            "co": {"max": 10.0},
            "pm25": {"max": 35.0},
            "nh3": {"max": 25.0},
            "airpresser": {"max": 1013.0}
        }
        with open(THRESHOLDS_FILE, 'w') as f:
            json.dump(default_thresholds, f, indent=4)
        return default_thresholds
    else:
        with open(THRESHOLDS_FILE, 'r') as f:
            return json.load(f)


# Hàm lưu ngưỡng vào tệp JSON
def save_thresholds(thresholds):
    with open(THRESHOLDS_FILE, 'w') as f:
        json.dump(thresholds, f, indent=4)


# Lớp cửa sổ cài đặt ngưỡng
class SettingsWindow(ctk.CTkToplevel):
    def __init__(self, parent):
        super().__init__(parent)
        self.title("Cài đặt ngưỡng")
        self.geometry("500x500")
        self.resizable(False, False)

        self.parent = parent
        self.thresholds = parent.thresholds  # Truy cập ngưỡng hiện tại

        # Tạo khung chính
        self.main_frame = ctk.CTkFrame(self, corner_radius=10)
        self.main_frame.pack(pady=20, padx=20, fill="both", expand=True)

        # Tạo biểu mẫu cài đặt ngưỡng cho từng thông số
        self.entries = {}
        row = 0
        for sensor in parent.sensors:
            key = sensor["key"]
            name = sensor["name"]

            label = ctk.CTkLabel(self.main_frame, text=name, font=("Helvetica", 12, "bold"))
            label.grid(row=row, column=0, pady=10, padx=10, sticky="w")

            if key in ["temp", "humi"]:
                # Thông số có cả ngưỡng dưới và trên
                min_label = ctk.CTkLabel(self.main_frame, text="Min:", font=("Helvetica", 10))
                min_label.grid(row=row, column=1, padx=5, sticky="e")
                min_entry = ctk.CTkEntry(self.main_frame, width=80)
                min_entry.grid(row=row, column=2, padx=5, sticky="w")
                min_entry.insert(0, str(self.thresholds[key].get("min", "")))

                max_label = ctk.CTkLabel(self.main_frame, text="Max:", font=("Helvetica", 10))
                max_label.grid(row=row, column=3, padx=5, sticky="e")
                max_entry = ctk.CTkEntry(self.main_frame, width=80)
                max_entry.grid(row=row, column=4, padx=5, sticky="w")
                max_entry.insert(0, str(self.thresholds[key].get("max", "")))

                self.entries[key] = {"min": min_entry, "max": max_entry}
            else:
                # Thông số chỉ có ngưỡng trên
                max_label = ctk.CTkLabel(self.main_frame, text="Max:", font=("Helvetica", 10))
                max_label.grid(row=row, column=1, padx=5, sticky="e")
                max_entry = ctk.CTkEntry(self.main_frame, width=80)
                max_entry.grid(row=row, column=2, padx=5, sticky="w")
                max_entry.insert(0, str(self.thresholds[key].get("max", "")))

                self.entries[key] = {"max": max_entry}

            row += 1

        # Nút lưu cài đặt
        self.save_button = ctk.CTkButton(
            self.main_frame,
            text="Lưu cài đặt",
            command=self.save_settings
        )
        self.save_button.grid(row=row, column=0, columnspan=5, pady=20)

    def save_settings(self):
        # Lấy giá trị từ các ô nhập và cập nhật ngưỡng
        for key, entry in self.entries.items():
            if key in ["temp", "humi"]:
                min_val = entry["min"].get()
                max_val = entry["max"].get()
                try:
                    min_val = float(min_val)
                except ValueError:
                    min_val = None
                try:
                    max_val = float(max_val)
                except ValueError:
                    max_val = None
                self.thresholds[key]["min"] = min_val
                self.thresholds[key]["max"] = max_val
            else:
                max_val = entry["max"].get()
                try:
                    max_val = float(max_val)
                except ValueError:
                    max_val = None
                self.thresholds[key]["max"] = max_val

        # Lưu ngưỡng vào tệp JSON
        save_thresholds(self.thresholds)

        # Cập nhật ngưỡng trong ứng dụng chính
        self.parent.thresholds = self.thresholds

        # Đóng cửa sổ cài đặt
        self.destroy()


# Lớp SensorApp kế thừa từ CTkFrame
class SensorApp(ctk.CTkFrame):
    def __init__(self, parent, database_path='database.db', *args, **kwargs):
        super().__init__(parent, *args, **kwargs)

        self.parent = parent
        self.database_path = database_path

        # Tải ngưỡng
        self.thresholds = load_thresholds()

        # Biến kiểm soát nhấp nháy
        self.blink_flags = {sensor: False for sensor in self.thresholds}
        self.blink_states = {sensor: False for sensor in self.thresholds}

        # Tạo khung để hiển thị ngày và giờ
        self.datetime_frame = ctk.CTkFrame(self, corner_radius=10)
        self.datetime_frame.pack(pady=10, padx=20, fill="x")

        self.date_label = ctk.CTkLabel(
            self.datetime_frame,
            text="Ngày: N/A",
            font=("Helvetica", 14)
        )
        self.date_label.pack(side="left", padx=10)

        self.time_label = ctk.CTkLabel(
            self.datetime_frame,
            text="Giờ: N/A",
            font=("Helvetica", 14)
        )
        self.time_label.pack(side="right", padx=10)

        # Tạo khung chính với lưới 2x3
        self.main_frame = ctk.CTkFrame(self, corner_radius=10)
        self.main_frame.pack(pady=20, padx=20, fill="both", expand=True)

        self.main_frame.grid_columnconfigure((0, 1, 2), weight=1)
        self.main_frame.grid_rowconfigure((0, 1), weight=1)

        # Danh sách các thông số và màu sắc tương ứng (chỉ bao gồm 6 cảm biến)
        self.sensors = [
            {"key": "temp", "name": "Nhiệt độ (°C)", "color": "#90EE90"},  # Xanh lá nhạt
            {"key": "humi", "name": "Độ ẩm (%)", "color": "#FFA07A"},  # Cam nhạt
            {"key": "co", "name": "CO (ppm)", "color": "#E6E6FA"},  # Tím nhạt
            {"key": "pm25", "name": "PM2.5 (µg/m³)", "color": "#FFD700"},  # Vàng
            {"key": "nh3", "name": "NH3 (ppm)", "color": "#87CEFA"},  # Xanh dương nhạt
            {"key": "airpresser", "name": "Áp suất không khí (hPa)", "color": "#98FB98"}  # Xanh lá mát
        ]

        self.labels = {}
        self.boxes = {}

        for index, sensor in enumerate(self.sensors):
            row = index // 3  # 0 hoặc 1
            col = index % 3  # 0,1,2

            # Tạo khung cho mỗi ô
            box = ctk.CTkFrame(
                self.main_frame,
                corner_radius=10,
                fg_color=sensor["color"],
                height=100
            )
            box.grid(row=row, column=col, padx=10, pady=10, sticky="nsew")
            self.boxes[sensor["key"]] = box

            # Tạo label tên thông số
            label_name = ctk.CTkLabel(
                box,
                text=sensor["name"],
                font=("Helvetica", 14, "bold"),
                text_color="black"
            )
            label_name.pack(pady=(10, 5))

            # Tạo label giá trị thông số
            label_value = ctk.CTkLabel(
                box,
                text="N/A",
                font=("Helvetica", 16),
                text_color="black"
            )
            label_value.pack(pady=(0, 10))

            self.labels[sensor["key"]] = label_value

        # Nút cập nhật thủ công
        self.update_button = ctk.CTkButton(
            self,
            text="Cập nhật ngay",
            command=self.update_data
        )
        self.update_button.pack(pady=10)

        # Nút mở cửa sổ cài đặt ngưỡng
        self.settings_button = ctk.CTkButton(
            self,
            text="Cài đặt ngưỡng",
            command=self.open_settings
        )
        self.settings_button.pack(pady=10)

        # Cập nhật dữ liệu tự động mỗi 5 giây
        self.update_data()
        self.after(5000, self.auto_update)

    def open_settings(self):
        SettingsWindow(self)

    def update_data(self):
        data = self.get_latest_measurement()
        if data:
            # Cập nhật ngày và giờ
            self.date_label.configure(text=f"Ngày: {data.get('date_save', 'N/A')}")
            self.time_label.configure(text=f"Giờ: {data.get('time_save', 'N/A')}")

            # Cập nhật các thông số cảm biến
            for sensor in self.sensors:
                key = sensor["key"]
                value = data.get(key, "N/A")
                self.labels[key].configure(text=str(value))

                # Kiểm tra ngưỡng
                threshold = self.thresholds.get(key, {})
                danger = False

                if key in ["temp", "humi"]:
                    min_val = threshold.get("min", None)
                    max_val = threshold.get("max", None)
                    if min_val is not None and value < min_val:
                        danger = True
                    if max_val is not None and value > max_val:
                        danger = True
                else:
                    max_val = threshold.get("max", None)
                    if max_val is not None and value > max_val:
                        danger = True

                if danger:
                    # Bắt đầu nhấp nháy
                    if not self.blink_flags[key]:
                        self.blink_flags[key] = True
                        self.blink_box(key)
                else:
                    # Kết thúc nhấp nháy và đặt màu nền bình thường
                    self.blink_flags[key] = False
                    self.boxes[key].configure(fg_color=sensor["color"])
        else:
            # Nếu không có dữ liệu, hiển thị "N/A"
            self.date_label.configure(text="Ngày: N/A")
            self.time_label.configure(text="Giờ: N/A")
            for sensor in self.sensors:
                key = sensor["key"]
                self.labels[key].configure(text="N/A")
                self.boxes[key].configure(fg_color=sensor["color"])

    def auto_update(self):
        self.update_data()
        self.after(1000, self.auto_update)  # Lặp lại sau mỗi 5000ms (5 giây)

    def blink_box(self, key):
        if not self.blink_flags[key]:
            # Nếu nhấp nháy đã bị dừng, đảm bảo màu nền bình thường
            self.boxes[key].configure(fg_color=self.get_sensor_color(key))
            return

        # Đảo trạng thái nhấp nháy
        self.blink_states[key] = not self.blink_states[key]
        if self.blink_states[key]:
            # Đổi màu nền thành đỏ để cảnh báo
            self.boxes[key].configure(fg_color="#FF6347")  # Tomato (đỏ cam)
        else:
            # Đổi màu nền về màu gốc
            self.boxes[key].configure(fg_color=self.get_sensor_color(key))

        # Gọi lại hàm này sau 500ms để tạo chu kỳ 1s
        self.after(500, lambda: self.blink_box(key))

    def get_sensor_color(self, key):
        for sensor in self.sensors:
            if sensor["key"] == key:
                return sensor["color"]
        return "#FFFFFF"  # Màu trắng mặc định

    def get_latest_measurement(self):
        try:
            conn = sqlite3.connect(self.database_path)
            cursor = conn.cursor()
            # Truy vấn để lấy dòng mới nhất dựa trên rowid
            cursor.execute("""
                SELECT date_save, time_save, temp, humi, co, pm25, nh3, airpresser 
                FROM measurements 
                ORDER BY rowid DESC 
                LIMIT 1
            """)
            row = cursor.fetchone()
            conn.close()
            if row:
                return {
                    "date_save": row[0],
                    "time_save": row[1],
                    "temp": row[2],
                    "humi": row[3],
                    "co": row[4],
                    "pm25": row[5],
                    "nh3": row[6],
                    "airpresser": row[7]
                }
            else:
                return {}
        except sqlite3.Error as e:
            print(f"Lỗi kết nối cơ sở dữ liệu: {e}")
            return {}

    def set_database(self, new_database_path):
        if new_database_path != self.database_path:
            self.database_path = new_database_path
            self.update_data()


# Ví dụ về cách nhúng SensorApp vào một ứng dụng khác với Radiobuttons
class MainApplication(ctk.CTk):
    def __init__(self):
        super().__init__()

        self.title("Ứng dụng chính")
        self.geometry("1000x700")
        self.resizable(False, False)

        # Tạo một khung trên cùng để chứa Radiobuttons
        self.top_frame = ctk.CTkFrame(self, corner_radius=10)
        self.top_frame.pack(pady=10, padx=20, fill="x")

        self.selected_db = ctk.StringVar()
        self.selected_db.set("database1.db")  # Mặc định chọn database1.db

        # Tạo Radiobuttons cho các database
        self.radiobutton1 = ctk.CTkRadioButton(
            self.top_frame,
            text="Database 1",
            variable=self.selected_db,
            value="database1.db",
            command=self.on_db_selection_changed
        )
        self.radiobutton1.pack(side="left", padx=10)

        self.radiobutton2 = ctk.CTkRadioButton(
            self.top_frame,
            text="Database 2",
            variable=self.selected_db,
            value="database2.db",
            command=self.on_db_selection_changed
        )
        self.radiobutton2.pack(side="left", padx=10)

        self.radiobutton3 = ctk.CTkRadioButton(
            self.top_frame,
            text="Database 3",
            variable=self.selected_db,
            value="database3.db",
            command=self.on_db_selection_changed
        )
        self.radiobutton3.pack(side="left", padx=10)

        # Tạo một khung chính để nhúng SensorApp
        self.main_frame = ctk.CTkFrame(self, corner_radius=10)
        self.main_frame.pack(pady=20, padx=20, fill="both", expand=True)

        # Nhúng SensorApp vào khung chính
        self.sensor_app = SensorApp(self.main_frame, database_path=self.selected_db.get())
        self.sensor_app.pack(fill="both", expand=True)

    def on_db_selection_changed(self):
        selected_db = self.selected_db.get()
        # Kiểm tra xem tệp database tồn tại
        if not os.path.exists(selected_db):
            messagebox.showerror(
                title="Lỗi",
                message=f"Tệp cơ sở dữ liệu '{selected_db}' không tồn tại."
            )
            # Reset selection to previous valid database or set to default
            # For simplicity, reset to 'database1.db' if selected_db is invalid
            # Alternatively, keep the previous valid selection
            # Here, reset to 'database1.db' hoặc kiểm tra các cơ sở dữ liệu còn lại
            if os.path.exists("database1.db"):
                self.selected_db.set("database1.db")
                # Force SensorApp to update
                self.sensor_app.set_database("database1.db")
            elif os.path.exists("database2.db"):
                self.selected_db.set("database2.db")
                self.sensor_app.set_database("database2.db")
            elif os.path.exists("database3.db"):
                self.selected_db.set("database3.db")
                self.sensor_app.set_database("database3.db")
            else:
                messagebox.showerror(
                    title="Lỗi",
                    message="Không tìm thấy bất kỳ cơ sở dữ liệu nào."
                )
        else:
            # Đặt database mới cho SensorApp
            self.sensor_app.set_database(selected_db)


# Chạy ứng dụng
if __name__ == "__main__":
    app = MainApplication()
    app.mainloop()
