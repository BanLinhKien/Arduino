import tkinter as tk
from tkinter import ttk, messagebox
import paho.mqtt.client as mqtt
from datetime import datetime
import queue
import threading

# Cấu hình MQTT
BROKER = "broker.hivemq.com"
PORT = 1883
TOPICS = [("ESP_SIM4G/DATA", 0), ("ESP_SIM4G/INFOR", 0)]

class MQTTApp(tk.Toplevel):
    def __init__(self, parent):
        super().__init__(parent)
        self.title("MQTT Client - Data")
        self.geometry("800x900")
        self.resizable(False, False)
        self.parent = parent

        # Font chung
        self.font = ("Arial", 12)

        # Queue để truyền dữ liệu từ callback MQTT sang giao diện
        self.queue = queue.Queue()

        # Trạng thái kết nối
        self.connected = False

        # Tạo giao diện
        self.create_widgets()

        # Khởi tạo MQTT client
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

        # Xử lý sự kiện đóng cửa sổ
        self.protocol("WM_DELETE_WINDOW", self.on_close)

        # Kết nối MQTT tự động ngay khi mở ứng dụng
        self.toggle_connection()

        # Định kỳ kiểm tra queue để cập nhật dữ liệu
        self.after(100, self.process_queue)

    def create_widgets(self):
        # Tạo khung chính
        main_frame = ttk.Frame(self, padding="10")
        main_frame.pack(fill=tk.BOTH, expand=True)

        # Nhãn trạng thái kết nối
        self.status_label = ttk.Label(main_frame, text="Chưa kết nối", font=("Arial", 14, "bold"), foreground="#ff4500")
        self.status_label.pack(pady=10)

        # Nút kết nối và xóa bảng
        button_frame = ttk.Frame(main_frame)
        button_frame.pack(pady=5)

        self.connect_button = ttk.Button(button_frame, text="Kết nối MQTT", command=self.toggle_connection)
        self.connect_button.pack(side=tk.LEFT, padx=5)

        self.clear_button = ttk.Button(button_frame, text="Xóa bảng", command=self.clear_table)
        self.clear_button.pack(side=tk.LEFT, padx=5)

        # Bảng hiển thị dữ liệu
        self.tree = ttk.Treeview(main_frame, columns=("Time", "Topic", "Content"), show='headings')
        self.tree.heading("Time", text="Thời gian")
        self.tree.heading("Topic", text="Topic")
        self.tree.heading("Content", text="Nội dung")
        self.tree.column("Time", width=170, anchor="center")
        self.tree.column("Topic", width=200, anchor="center")
        self.tree.column("Content", width=600, anchor="w")

        # Thêm thanh cuộn cho bảng
        scrollbar = ttk.Scrollbar(main_frame, orient=tk.VERTICAL, command=self.tree.yview)
        self.tree.configure(yscroll=scrollbar.set)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        self.tree.pack(fill=tk.BOTH, expand=True)

        style = ttk.Style()
        style.configure("Treeview", rowheight=110, background="#ffffff", foreground="black", fieldbackground="#ffffff",
                        font=("Courier", 10))
        style.map("Treeview", background=[('selected', '#4682b4')])

    def toggle_connection(self):
        if not self.connected:
            # Kết nối MQTT
            try:
                self.client.connect(BROKER, PORT, 60)
                self.client.subscribe(TOPICS)
                self.client.loop_start()
                self.status_label.config(text="Đang kết nối... (Chờ nhận dữ liệu)", foreground="#32cd32")
                self.connected = True
                self.connect_button.config(text="Ngắt kết nối MQTT")
            except Exception as e:
                messagebox.showerror("Lỗi", f"Lỗi kết nối: {str(e)}")
        else:
            # Ngắt kết nối
            self.client.loop_stop()
            self.client.disconnect()
            self.status_label.config(text="Đã ngắt kết nối", foreground="#ff4500")
            self.connected = False
            self.connect_button.config(text="Kết nối MQTT")
            self.clear_table()

    def clear_table(self):
        for item in self.tree.get_children():
            self.tree.delete(item)

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            self.queue.put(("status", "Kết nối thành công", "#32cd32"))
        else:
            self.queue.put(("status", f"Lỗi kết nối: {rc}", "#ff4500"))

    def on_message(self, client, userdata, msg):
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        message = msg.payload.decode()
        topic = msg.topic
        self.queue.put(("data", timestamp, topic, message))

    def process_queue(self):
        try:
            while True:
                item = self.queue.get_nowait()
                if item[0] == "status":
                    _, text, color = item
                    self.status_label.config(text=text, foreground=color)
                elif item[0] == "data":
                    _, timestamp, topic, message = item
                    self.tree.insert("", "end", values=(timestamp, topic, message))
                    # Cuộn xuống cuối
                    self.tree.yview_moveto(1)
        except queue.Empty:
            pass
        # Định kỳ kiểm tra lại
        self.after(100, self.process_queue)

    def on_close(self):
        if self.connected:
            self.toggle_connection()
        self.destroy()

class MainApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Ứng dụng Chính")
        self.geometry("400x200")
        self.resizable(False, False)

        # Tạo giao diện chính
        self.create_widgets()

    def create_widgets(self):
        main_frame = ttk.Frame(self, padding="20")
        main_frame.pack(fill=tk.BOTH, expand=True)

        open_mqtt_button = ttk.Button(main_frame, text="Mở MQTT Client", command=self.open_mqtt_client)
        open_mqtt_button.pack(pady=50)

    def open_mqtt_client(self):
        # Kiểm tra xem cửa sổ MQTTApp đã được mở chưa
        if hasattr(self, 'mqtt_window') and self.mqtt_window.winfo_exists():
            self.mqtt_window.lift()
        else:
            self.mqtt_window = MQTTApp(self)

def main():
    app = MainApp()
    app.mainloop()

if __name__ == "__main__":
    main()
