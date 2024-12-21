from customtkinter import *
from login import LoginForm  # Import lớp LoginForm
from PIL import Image
from db_excel import *
from datetime import datetime
import random
from realtime_chart import *
from day_chart import *
from month_chart import *
from db_excel import *
from lastest_data_show import *
from map_app import *
from mqtt_app import *
from auto_gen_data import *
class MainApp(CTk):
    def __init__(self):
        super().__init__()
        self.title("Chương trình chính")
        self.geometry("400x300")
        set_appearance_mode("dark")  # Modes: system (default), light, dark
        set_default_color_theme("blue")

        self.day_choose_bieu_do_ngay = ""

        # Hiện form đăng nhập
        self.show_login()

    def show_login(self):
        # Mở cửa sổ đăng nhập
        login_form = LoginForm(on_success=self.open_dashboard)
        login_form.mainloop()

    def open_dashboard(self):
        broker = "broker.hivemq.com"
        port = 1883
        topic = "ESP_SIM4G/DATA"

        # Create the MQTT client handler and connect to the broker
        mqtt_handler = MQTTClientHandler(broker, port, topic)
        mqtt_handler.connect()
        mqtt_handler.start()
        # Tạo cửa sổ Dashboard
        root = CTkToplevel(self)
        set_appearance_mode("dark")  # Modes: system (default), light, dark
        set_default_color_theme("blue")  # Themes: blue (default), dark-blue, green
        root.title("Đỗ Thế Dương_62TĐH-HTN")
        root.geometry("1750x800")

        frame_left = CTkFrame(root)
        frame_left.grid(column=0, row=0, padx=10, pady=6, sticky="NW")
        frame_left_1 = CTkFrame(frame_left)
        frame_left_1.grid(column=0, row=0, padx=10, pady=6)
        # TODO Create image
        my_image = CTkImage(light_image=Image.open("logo.png"),
                            dark_image=Image.open("logo.png"),
                            size=(220,200))
        # TODO Tao lable de chua anh
        my_logo = CTkLabel(frame_left_1,
                           text="",
                           image=my_image)
        my_logo.grid(column=0, row=0, padx=10, pady=6)

        my_topic = CTkLabel(frame_left_1,
                            text="Ứng Dụng IoT Trong Quản Lý\n Giám Sát Trạm Xử Lý Rác Thải ",
                            font=("Arial", 16),
                            anchor="center")
        my_topic.grid(column=0, row=1, padx=10, pady=6, sticky="W")

        my_name = CTkLabel(frame_left_1,
                           text="Đỗ Thế Dương",
                           font=("Arial", 16))
        my_name.grid(column=0, row=2, padx=10, pady=6, sticky="W")
        my_class = CTkLabel(frame_left_1,
                            text="Lớp 62TĐH-HTN",
                            font=("Arial", 16))
        my_class.grid(column=0, row=3, padx=10, pady=6, sticky="W")
        my_school = CTkLabel(frame_left_1,
                             text="Trường Đại học Thủy Lợi",
                             font=("Arial", 16))
        my_school.grid(column=0, row=4, padx=10, pady=6, sticky="W")

        # Frame cho radio chọn khu vực
        frame_left_2 = CTkFrame(frame_left)
        frame_left_2.grid(column=0, row=1, padx=10, pady=(10, 20), ipadx=33, ipady=20)

        radiobt_lable = CTkLabel(frame_left_2, text="Chọn Khu Vực",
                                 font=("Arial", 16))
        radiobt_lable.grid(column=0, row=0, padx=5, pady=(20, 10))


        selected_dbk = ctk.StringVar()
        selected_dbk.set("database1.db")  # Mặc định chọn database1.db

        def on_db_selection_changed():
            selected_db = selected_dbk.get()
            print(selected_db)

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
                    selected_dbk.set("database1.db")
                    # Force SensorApp to update
                    sensor_app.set_database("database1.db")
                elif os.path.exists("database2.db"):
                    selected_dbk.set("database2.db")
                    sensor_app.set_database("database2.db")
                elif os.path.exists("database3.db"):
                    selected_dbk.set("database3.db")
                    sensor_app.set_database("database3.db")
                else:
                    messagebox.showerror(
                        title="Lỗi",
                        message="Không tìm thấy bất kỳ cơ sở dữ liệu nào."
                    )
            else:
                # Đặt database mới cho SensorApp
                sensor_app.set_database(selected_db)
                display_realtime_chart_app.set_database(selected_db)
                display_day_chart_frame.set_database(selected_db)
                display_month_chart_app.switch_database(selected_db)
                display_database_excel.set_database(selected_db)
                display_map_app.update_image(selected_db)

        # Tạo Radiobuttons cho các database
        radiobutton1 = ctk.CTkRadioButton(
            frame_left_2,
            text="Khu Vực 1",
            variable=selected_dbk,
            value="database1.db",
            command=on_db_selection_changed
        )
        radiobutton1.grid(column=0, row=1, padx=5, pady=10)

        radiobutton2 = ctk.CTkRadioButton(
            frame_left_2,
            text="Khu Vực 2",
            variable=selected_dbk,
            value="database2.db",
            command=on_db_selection_changed
        )
        radiobutton2.grid(column=0, row=2, padx=5, pady=10)

        radiobutton3 = ctk.CTkRadioButton(
            frame_left_2,
            text="Khu Vực 3",
            variable=selected_dbk,
            value="database3.db",
            command=on_db_selection_changed
        )
        radiobutton3.grid(column=0, row=3, padx=5, pady=10)

        # Nút nhấn đăng xuất
        def button_logout_handle():
            root.quit()

        frame_button = CTkFrame(root)
        frame_button.place(relx=0, rely=1, x=10, y=-20, anchor="sw")

        button_logout = CTkButton(frame_button, text="Đăng Xuất",
                                  command=button_logout_handle,
                                  font=("Arial", 12),
                                  corner_radius=0,
                                  height=35,
                                  width=190)
        button_logout.grid(column=0, row=1, padx=10, pady=6, sticky="S")


        def button_mqtt_handle():
            # Kiểm tra xem cửa sổ MQTTApp đã được mở chưa
            mqtt_window = MQTTApp(root)

        button_mqtt = CTkButton(frame_button, text="Dữ Liệu MQTT",
                                  command=button_mqtt_handle,
                                  font=("Arial", 12),
                                  corner_radius=0,
                                  height=35,
                                  width=190)

        button_mqtt.grid(column=0, row=0, padx=10, pady=6, sticky="S")

        #######################################################################
        # TODO Tạo tabview
        frame_right = CTkFrame(root)
        frame_right.grid(column=1, row=0, padx=10, pady=10, sticky="NW")
        tab_view = CTkTabview(frame_right,
                              anchor="center",
                              width=1400,
                              height=800,
                              corner_radius=5,
                              border_width=1
                              )
        # create tabs
        tab_1 = tab_view.add("Dữ Liệu Realtime")
        tab_2 = tab_view.add("Biểu Đồ Realtime")
        tab_3 = tab_view.add("Biểu Đồ ngày")
        tab_4 = tab_view.add("Biểu Đồ tháng")
        tab_5 = tab_view.add("Xuất Excel")
        tab_6 = tab_view.add("Map")

        tab_view.grid(row=0, column=0, padx=20, pady=10)

        sensor_app = SensorApp(tab_1, database_path=selected_dbk.get())
        sensor_app.pack(fill="both")

        display_realtime_chart_app = RealtimeChartFrame(tab_2)
        display_realtime_chart_app.pack(fill="both")

        display_day_chart_frame = DayChartFrame(tab_3)
        display_day_chart_frame.pack(fill="both", expand=True)

        display_month_chart_app = MonthChartApp(tab_4)
        display_month_chart_app.pack(fill="both", expand=True)

        display_database_excel = DatabaseExcel(tab_5)
        display_database_excel.pack(fill="both", expand=True)

        display_map_app = ImageRadioApp(tab_6)
        # display_map_app.pack(fill="both")


# Chạy chương trình
if __name__ == "__main__":
    app = MainApp()
    app.mainloop()
