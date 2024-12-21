# import customtkinter as ctk
# import pandas as pd
# import matplotlib.pyplot as plt
# from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
# import sqlite3  # Import sqlite3 for database operations
# from datetime import datetime
# import mplcursors
# import matplotlib.dates as mdates
# from tkcalendar import DateEntry  # Import DateEntry for date selection
#
#
# class ParameterChart:
#     def __init__(self, master, param_name, unit, color):
#         """
#         Initialize a chart for a specific parameter.
#
#         :param master: Frame containing the chart.
#         :param param_name: Name of the parameter.
#         :param unit: Unit of the parameter.
#         :param color: Color of the line chart.
#         """
#         self.master = master
#         self.param_name = param_name
#         self.unit = unit
#         self.color = color
#
#         # Initialize figure and axis
#         self.fig, self.ax = plt.subplots(figsize=(6, 4), dpi=100)
#         self.line, = self.ax.plot([], [], color=self.color, marker='o', linestyle='-', linewidth=2, label=param_name)
#         self.ax.set_title(f'{self.param_name} ({self.unit})', fontsize=12, weight='bold')
#         self.ax.set_xlabel('Time', fontsize=10)
#         self.ax.set_ylabel(self.unit, fontsize=10)
#         self.ax.grid(True, linestyle='--', alpha=0.7)
#         self.ax.legend(fontsize=8)
#
#         # Format x-axis for dates
#         self.ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M:%S'))
#         self.ax.xaxis.set_major_locator(mdates.AutoDateLocator())
#
#         self.fig.autofmt_xdate()  # Auto-format date labels
#
#         self.fig.tight_layout()
#
#         # Embed the chart into Tkinter
#         self.canvas = FigureCanvasTkAgg(self.fig, master=self.master)
#         self.canvas.draw()
#         self.canvas.get_tk_widget().pack(fill='both', expand=True)
#
#         # Initialize data containers
#         self.x_data = []
#         self.y_data = []
#
#         # Add interactive tooltips
#         self.add_tooltips()
#
#     def update_chart(self, x, y):
#         """
#         Update the chart with new data.
#
#         :param x: List of datetime objects for the x-axis.
#         :param y: List of values for the y-axis.
#         """
#         self.x_data = x
#         self.y_data = y
#
#         self.line.set_data(self.x_data, self.y_data)
#         self.ax.relim()
#         self.ax.autoscale_view()
#
#         # Update the x-axis limits to fit the data
#         if self.x_data:
#             self.ax.set_xlim(min(self.x_data), max(self.x_data))
#
#         self.canvas.draw()
#
#     def add_tooltips(self):
#         """
#         Add interactive tooltips for the line chart.
#         """
#         cursor = mplcursors.cursor(self.line, hover=True)
#
#         @cursor.connect("add")
#         def on_add(sel):
#             try:
#                 # Ensure sel.index is an integer
#                 index = int(sel.index)
#
#                 # Prevent IndexError if index is out of bounds
#                 if index >= len(self.x_data) or index < 0:
#                     return
#
#                 time_str = self.x_data[index].strftime('%H:%M:%S')
#                 value = self.y_data[index]
#                 sel.annotation.set(text=f"Time: {time_str}\nValue: {value} {self.unit}")
#                 sel.annotation.get_bbox_patch().set(fc="white")
#             except Exception as e:
#                 print(f"Error handling tooltip: {e}")
#
#
# class RealtimeChartApp:
#     def __init__(self, root):
#         """
#         Initialize the environment parameters chart application.
#
#         :param root: Tkinter root window.
#         """
#         self.root = root
#         self.root.title("Biểu đồ Các Thông Số Môi Trường - 20 Giá Trị Gần Nhất")
#         self.root.geometry("1300x800")
#         self.root.resizable(False, False)  # Prevent window resizing (optional)
#
#         # Define parameters to measure, their units, ranges, and colors
#         self.parameters = {
#             'Temperature': {'unit': '°C', 'color': 'red'},
#             'Humidity': {'unit': '%RH', 'color': 'blue'},
#             'Dust Sensor': {'unit': 'µg/m³', 'color': 'green'},
#             'CO': {'unit': 'ppm', 'color': 'purple'},
#             'NH3': {'unit': 'ppm', 'color': 'orange'},
#             'Air Pressure': {'unit': 'hPa', 'color': 'brown'},
#         }
#
#         # Mapping from parameter names to database column names
#         self.param_to_db_column = {
#             'Temperature': 'temp',
#             'Humidity': 'humi',
#             'Dust Sensor': 'pm25',
#             'CO': 'co',
#             'NH3': 'nh3',
#             'Air Pressure': 'airpresser',
#         }
#
#         # Initialize selected date to today (not used in this version)
#         self.selected_date = datetime.now().date()
#
#         # Connect to the SQLite database
#         self.connect_to_db()
#
#         # # Create a control frame above the main_frame for potential controls
#         # self.control_frame = ctk.CTkFrame(self.root)
#         # self.control_frame.pack(padx=20, pady=10, fill='x')
#
#         # (Optional) You can add controls here if needed in future
#
#         # Create a scrollable frame to contain all the charts
#         self.main_frame = ctk.CTkScrollableFrame(
#             self.root,
#             width=1250,
#             height=700,
#             border_width=2,
#             border_color="grey",
#             corner_radius=10
#         )
#         self.main_frame.pack(padx=20, pady=20)
#
#         # Configure grid layout within the scrollable frame
#         for i in range(3):
#             self.main_frame.grid_rowconfigure(i, weight=1, pad=10)
#         for j in range(2):
#             self.main_frame.grid_columnconfigure(j, weight=1, pad=10)
#
#         # Position of each parameter in the grid
#         self.grid_positions = {
#             'Temperature': (0, 0),
#             'Humidity': (0, 1),
#             'Dust Sensor': (1, 0),
#             'CO': (1, 1),
#             'NH3': (2, 0),
#             'Air Pressure': (2, 1),
#         }
#
#         # Create chart instances for each parameter
#         self.charts = {}
#         self.create_charts()
#
#         # Start the periodic data update
#         self.update_charts()
#
#     def connect_to_db(self):
#         """
#         Establish a connection to the SQLite database.
#         """
#         try:
#             self.conn = sqlite3.connect('database.db', check_same_thread=False)
#             self.cursor = self.conn.cursor()
#             print("Connected to database.db successfully.")
#         except sqlite3.Error as e:
#             print(f"Error connecting to database: {e}")
#             self.conn = None
#             self.cursor = None
#
#     def fetch_latest_data(self):
#         """
#         Fetch the latest 20 entries from the database.
#
#         :return: DataFrame containing the latest 20 entries.
#         """
#         if not self.conn or not self.cursor:
#             print("Database connection is not established.")
#             return pd.DataFrame()
#
#         try:
#             # Fetch the latest 20 entries ordered by date_save and time_save descending
#             # Convert date_save from dd/mm/yyyy to yyyy-mm-dd for proper sorting
#             query = """
#                 SELECT date_save, time_save, temp, humi, co, pm25, nh3, airpresser
#                 FROM measurements
#                 ORDER BY
#                     substr(date_save,7,4) || '-' || substr(date_save,4,2) || '-' || substr(date_save,1,2) DESC,
#                     time_save DESC
#                 LIMIT 20
#             """
#             self.cursor.execute(query)
#             rows = self.cursor.fetchall()
#
#             if not rows:
#                 print("No data found in the database.")
#                 return pd.DataFrame()
#
#             # Create DataFrame
#             df = pd.DataFrame(rows, columns=['date_save', 'time_save', 'temp', 'humi', 'co', 'pm25', 'nh3', 'airpresser'])
#
#             # Combine date and time into a single datetime column
#             df['timestamp'] = pd.to_datetime(df['date_save'] + ' ' + df['time_save'], format='%d/%m/%Y %H:%M:%S')
#
#             # Sort by timestamp ascending for proper plotting
#             df = df.sort_values('timestamp')
#
#             return df
#
#         except sqlite3.Error as e:
#             print(f"Error fetching data from database: {e}")
#             return pd.DataFrame()
#
#     def process_data(self, df):
#         """
#         Process the fetched data.
#
#         :param df: DataFrame containing the latest 20 entries.
#         :return: Dictionary of DataFrames for each parameter.
#         """
#         if df.empty:
#             return {param: pd.DataFrame() for param in self.parameters}
#
#         data = {}
#         for param, info in self.parameters.items():
#             column = self.param_to_db_column[param]
#             data[param] = df[['timestamp', column]].dropna()
#
#         return data
#
#     def create_charts(self):
#         """
#         Create chart instances and embed them into the grid.
#         """
#         for param, info in self.parameters.items():
#             position = self.grid_positions[param]
#             frame = ctk.CTkFrame(self.main_frame, width=500, height=400)
#             frame.grid(row=position[0], column=position[1], padx=10, pady=10, sticky="nsew")
#             frame.grid_propagate(False)  # Prevent frame from resizing based on content
#
#             # Initialize ParameterChart without data
#             chart = ParameterChart(
#                 master=frame,
#                 param_name=param,
#                 unit=info['unit'],
#                 color=info['color']
#             )
#             self.charts[param] = chart
#
#     def update_charts(self):
#         """
#         Fetch new data and update all charts. Schedule the next update.
#         """
#         df = self.fetch_latest_data()
#         processed_data = self.process_data(df)
#
#         for param, chart in self.charts.items():
#             data = processed_data.get(param)
#             if data is not None and not data.empty:
#                 x = data['timestamp'].tolist()
#                 y = data[self.param_to_db_column[param]].tolist()
#                 chart.update_chart(x, y)
#             else:
#                 # If no data, clear the chart
#                 chart.update_chart([], [])
#
#         # Schedule the next update after 5000 ms (5 seconds)
#         self.root.after(10000, self.update_charts)
#
#     def run(self):
#         """
#         Start the main loop of the application.
#         """
#         self.root.mainloop()
#         # Close the database connection when the application is closed
#         if self.conn:
#             self.conn.close()
#             print("Database connection closed.")
#
#
# # def main():
# #     # Initialize CustomTkinter
# #     ctk.set_appearance_mode("System")  # Other options: "Dark", "Light"
# #     ctk.set_default_color_theme("blue")  # You can change the theme
# #
# #     # Create the main window
# #     root = ctk.CTk()
# #
# #     # Create the application
# #     app = EnvironmentChartApp(root)
# #
# #     # Run the application
# #     app.run()
# #
# #
# # if __name__ == "__main__":
# #     main()
# # ở control_frame , hãy thêm lựa chọn số phần tử hiển thị lấy từ database


##################################################################################

# import customtkinter as ctk
# from tkinter import ttk, messagebox, filedialog
# import sqlite3
# from datetime import datetime
# import pandas as pd
# import matplotlib.pyplot as plt
# from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
# import mplcursors
# import matplotlib.dates as mdates
# from tkcalendar import DateEntry
#
#
# class ParameterChart:
#     def __init__(self, master, param_name, unit, color):
#         """
#         Initialize a chart for a specific parameter.
#
#         :param master: Frame containing the chart.
#         :param param_name: Name of the parameter.
#         :param unit: Unit of the parameter.
#         :param color: Color of the line chart.
#         """
#         self.master = master
#         self.param_name = param_name
#         self.unit = unit
#         self.color = color
#
#         # Initialize figure and axis
#         self.fig, self.ax = plt.subplots(figsize=(6, 4), dpi=100)
#         self.line, = self.ax.plot([], [], color=self.color, marker='o', linestyle='-', linewidth=2, label=param_name)
#         self.ax.set_title(f'{self.param_name} ({self.unit})', fontsize=12, weight='bold')
#         self.ax.set_xlabel('Time', fontsize=10)
#         self.ax.set_ylabel(self.unit, fontsize=10)
#         self.ax.grid(True, linestyle='--', alpha=0.7)
#         self.ax.legend(fontsize=8)
#
#         # Format x-axis for dates
#         self.ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M:%S'))
#         self.ax.xaxis.set_major_locator(mdates.AutoDateLocator())
#
#         self.fig.autofmt_xdate()  # Auto-format date labels
#
#         self.fig.tight_layout()
#
#         # Embed the chart into Tkinter
#         self.canvas = FigureCanvasTkAgg(self.fig, master=self.master)
#         self.canvas.draw()
#         self.canvas.get_tk_widget().pack(fill='both', expand=True)
#
#         # Initialize data containers
#         self.x_data = []
#         self.y_data = []
#
#         # Add interactive tooltips
#         self.add_tooltips()
#
#     def update_chart(self, x, y):
#         """
#         Update the chart with new data.
#
#         :param x: List of datetime objects for the x-axis.
#         :param y: List of values for the y-axis.
#         """
#         self.x_data = x
#         self.y_data = y
#
#         self.line.set_data(self.x_data, self.y_data)
#         self.ax.relim()
#         self.ax.autoscale_view()
#
#         # Update the x-axis limits to fit the data
#         if self.x_data:
#             self.ax.set_xlim(min(self.x_data), max(self.x_data))
#
#         self.canvas.draw()
#
#     def add_tooltips(self):
#         """
#         Add interactive tooltips for the line chart.
#         """
#         cursor = mplcursors.cursor(self.line, hover=True)
#
#         @cursor.connect("add")
#         def on_add(sel):
#             try:
#                 # Ensure sel.index is an integer
#                 index = int(sel.index)
#
#                 # Prevent IndexError if index is out of bounds
#                 if index >= len(self.x_data) or index < 0:
#                     return
#
#                 time_str = self.x_data[index].strftime('%H:%M:%S')
#                 value = self.y_data[index]
#                 sel.annotation.set(text=f"Time: {time_str}\nValue: {value} {self.unit}")
#                 sel.annotation.get_bbox_patch().set(fc="white")
#             except Exception as e:
#                 print(f"Error handling tooltip: {e}")
#
#
# class RealtimeChartFrame(ctk.CTkFrame):
#     def __init__(self, parent):
#         """
#         Initialize the environment parameters chart frame.
#
#         :param parent: Parent widget.
#         """
#         super().__init__(parent)
#         self.parent = parent
#         self.pack(fill="both", expand=True)
#
#         # Define parameters to measure, their units, and colors
#         self.parameters = {
#             'Temperature': {'unit': '°C', 'color': 'red'},
#             'Humidity': {'unit': '%RH', 'color': 'blue'},
#             'Dust Sensor': {'unit': 'µg/m³', 'color': 'green'},
#             'CO': {'unit': 'ppm', 'color': 'purple'},
#             'NH3': {'unit': 'ppm', 'color': 'orange'},
#             'Air Pressure': {'unit': 'hPa', 'color': 'brown'},
#         }
#
#         # Mapping from parameter names to database column names
#         self.param_to_db_column = {
#             'Temperature': 'temp',
#             'Humidity': 'humi',
#             'Dust Sensor': 'pm25',
#             'CO': 'co',
#             'NH3': 'nh3',
#             'Air Pressure': 'airpresser',
#         }
#
#         # Initialize selected date to today (not used in this version)
#         self.selected_date = datetime.now().date()
#
#         # Connect to the SQLite database
#         self.connect_to_db()
#
#         # Create a scrollable frame to contain all the charts
#         self.main_frame = ctk.CTkScrollableFrame(
#             self,
#             width=1250,
#             height=700,
#             border_width=2,
#             border_color="grey",
#             corner_radius=10
#         )
#         self.main_frame.pack(padx=20, pady=20, fill="both", expand=True)
#
#         # Configure grid layout within the scrollable frame
#         for i in range(3):
#             self.main_frame.grid_rowconfigure(i, weight=1, pad=10)
#         for j in range(2):
#             self.main_frame.grid_columnconfigure(j, weight=1, pad=10)
#
#         # Position of each parameter in the grid
#         self.grid_positions = {
#             'Temperature': (0, 0),
#             'Humidity': (0, 1),
#             'Dust Sensor': (1, 0),
#             'CO': (1, 1),
#             'NH3': (2, 0),
#             'Air Pressure': (2, 1),
#         }
#
#         # Create chart instances for each parameter
#         self.charts = {}
#         self.create_charts()
#
#         # Start the periodic data update
#         self.update_charts()
#
#     def connect_to_db(self):
#         """
#         Establish a connection to the SQLite database.
#         """
#         try:
#             self.conn = sqlite3.connect('database.db', check_same_thread=False)
#             self.cursor = self.conn.cursor()
#             print("Connected to database.db successfully.")
#         except sqlite3.Error as e:
#             print(f"Error connecting to database: {e}")
#             self.conn = None
#             self.cursor = None
#
#     def fetch_latest_data(self):
#         """
#         Fetch the latest 20 entries from the database.
#
#         :return: DataFrame containing the latest 20 entries.
#         """
#         if not self.conn or not self.cursor:
#             print("Database connection is not established.")
#             return pd.DataFrame()
#
#         try:
#             # Fetch the latest 20 entries ordered by date_save and time_save descending
#             # Convert date_save from dd/mm/yyyy to yyyy-mm-dd for proper sorting
#             query = """
#                 SELECT date_save, time_save, temp, humi, co, pm25, nh3, airpresser
#                 FROM measurements
#                 ORDER BY
#                     substr(date_save,7,4) || '-' || substr(date_save,4,2) || '-' || substr(date_save,1,2) DESC,
#                     time_save DESC
#                 LIMIT 20
#             """
#             self.cursor.execute(query)
#             rows = self.cursor.fetchall()
#
#             if not rows:
#                 print("No data found in the database.")
#                 return pd.DataFrame()
#
#             # Create DataFrame
#             df = pd.DataFrame(rows, columns=['date_save', 'time_save', 'temp', 'humi', 'co', 'pm25', 'nh3', 'airpresser'])
#
#             # Combine date and time into a single datetime column
#             df['timestamp'] = pd.to_datetime(df['date_save'] + ' ' + df['time_save'], format='%d/%m/%Y %H:%M:%S')
#
#             # Sort by timestamp ascending for proper plotting
#             df = df.sort_values('timestamp')
#
#             return df
#
#         except sqlite3.Error as e:
#             print(f"Error fetching data from database: {e}")
#             return pd.DataFrame()
#
#     def process_data(self, df):
#         """
#         Process the fetched data.
#
#         :param df: DataFrame containing the latest 20 entries.
#         :return: Dictionary of DataFrames for each parameter.
#         """
#         if df.empty:
#             return {param: pd.DataFrame() for param in self.parameters}
#
#         data = {}
#         for param, info in self.parameters.items():
#             column = self.param_to_db_column[param]
#             data[param] = df[['timestamp', column]].dropna()
#
#         return data
#
#     def create_charts(self):
#         """
#         Create chart instances and embed them into the grid.
#         """
#         for param, info in self.parameters.items():
#             position = self.grid_positions[param]
#             frame = ctk.CTkFrame(self.main_frame, width=500, height=400)
#             frame.grid(row=position[0], column=position[1], padx=10, pady=10, sticky="nsew")
#             frame.grid_propagate(False)  # Prevent frame from resizing based on content
#
#             # Initialize ParameterChart without data
#             chart = ParameterChart(
#                 master=frame,
#                 param_name=param,
#                 unit=info['unit'],
#                 color=info['color']
#             )
#             self.charts[param] = chart
#
#     def update_charts(self):
#         """
#         Fetch new data and update all charts. Schedule the next update.
#         """
#         df = self.fetch_latest_data()
#         processed_data = self.process_data(df)
#
#         for param, chart in self.charts.items():
#             data = processed_data.get(param)
#             if data is not None and not data.empty:
#                 x = data['timestamp'].tolist()
#                 y = data[self.param_to_db_column[param]].tolist()
#                 chart.update_chart(x, y)
#             else:
#                 # If no data, clear the chart
#                 chart.update_chart([], [])
#
#         # Schedule the next update after 10000 ms (10 seconds)
#         self.after(10000, self.update_charts)
#
#     def export_to_excel(self):
#         """
#         Export the currently displayed data to an Excel file.
#         """
#         # Lấy tất cả các bản ghi hiện tại trong Treeview
#         records = []
#         for row_id in self.tree.get_children():
#             row = self.tree.item(row_id)['values']
#             records.append(row)
#
#         if not records:
#             messagebox.showwarning("Cảnh Báo", "Không có dữ liệu để xuất.")
#             return
#
#         # Tạo DataFrame từ dữ liệu
#         df = pd.DataFrame(records, columns=["Ngày", "Giờ", "Nhiệt Độ", "Độ Ẩm", "CO", "PM2.5", "NH3", "Áp Suất Không Khí"])
#
#         # Mở hộp thoại để chọn nơi lưu tệp Excel
#         file_path = filedialog.asksaveasfilename(
#             defaultextension=".xlsx",
#             filetypes=[("Excel files", "*.xlsx"), ("All files", "*.*")],
#             title="Lưu Dữ Liệu Thành Excel"
#         )
#         if not file_path:
#             return  # Người dùng đã hủy bỏ hộp thoại
#
#         try:
#             # Lưu DataFrame vào tệp Excel
#             df.to_excel(file_path, index=False)
#             messagebox.showinfo("Thành Công", f"Dữ liệu đã được lưu thành công tại:\n{file_path}")
#         except Exception as e:
#             messagebox.showerror("Lỗi", f"Đã xảy ra lỗi khi lưu tệp Excel:\n{e}")
#
#     def on_closing(self):
#         """
#         Handle the closing of the frame/application.
#         """
#         if self.conn:
#             self.conn.close()
#             print("Database connection closed.")
#         self.destroy()
#
#
# # Thiết lập giao diện
# ctk.set_appearance_mode("System")  # Có thể chọn "System", "Dark", "Light"
# ctk.set_default_color_theme("blue")  # Có thể chọn "blue", "green", "dark-blue"
#
# class MainApp(ctk.CTk):
#     def __init__(self):
#         super().__init__()
#         self.title("Chương Trình Chính")
#         self.geometry("1400x900")  # Kích thước tùy chỉnh cho chương trình chính
#
#         # Tạo một Frame để chứa RealtimeChartFrame
#         main_frame = ctk.CTkFrame(self)
#         main_frame.pack(padx=20, pady=20, fill="both", expand=True)
#
#         # Tạo và nhúng RealtimeChartFrame vào main_frame
#         self.realtime_chart = RealtimeChartFrame(main_frame)
#         self.realtime_chart.pack(fill="both", expand=True)
#
#         # Thêm các widget khác của chương trình chính nếu cần
#         # Ví dụ: Một menu, sidebar, etc.
#         self.create_menu()
#
#     def create_menu(self):
#         """
#         Tạo menu cho chương trình chính.
#         """
#         # Tạo một menu bar
#         menu_bar = ctk.CTkOptionMenu(
#             self,
#             values=["Tùy chọn 1", "Tùy chọn 2"],
#             command=self.menu_callback,
#             width=150,
#             height=30
#         )
#         menu_bar.pack(pady=10)
#
#     def menu_callback(self, choice):
#         """
#         Xử lý sự kiện khi một tùy chọn trong menu được chọn.
#
#         :param choice: Tùy chọn được chọn.
#         """
#         messagebox.showinfo("Menu", f"Bạn đã chọn: {choice}")
#
#     def on_closing(self):
#         """
#         Xử lý khi đóng ứng dụng chính.
#         """
#         # Đóng kết nối cơ sở dữ liệu nếu cần
#         try:
#             self.realtime_chart.conn.close()
#         except:
#             pass
#         self.destroy()
#
# if __name__ == "__main__":
#     app = MainApp()
#     app.protocol("WM_DELETE_WINDOW", app.on_closing)
#     app.mainloop()



import customtkinter as ctk
from tkinter import ttk, messagebox, filedialog
import sqlite3
from datetime import datetime
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import mplcursors
import matplotlib.dates as mdates
from tkcalendar import DateEntry

class ParameterChart:
    def __init__(self, master, param_name, unit, color):
        """
        Initialize a chart for a specific parameter.

        :param master: Frame containing the chart.
        :param param_name: Name of the parameter.
        :param unit: Unit of the parameter.
        :param color: Color of the line chart.
        """
        self.master = master
        self.param_name = param_name
        self.unit = unit
        self.color = color

        # Initialize figure and axis
        self.fig, self.ax = plt.subplots(figsize=(6, 4), dpi=100)
        self.line, = self.ax.plot([], [], color=self.color, marker='o', linestyle='-', linewidth=2, label=param_name)
        self.ax.set_title(f'{self.param_name} ({self.unit})', fontsize=12, weight='bold')
        self.ax.set_xlabel('Time', fontsize=10)
        self.ax.set_ylabel(self.unit, fontsize=10)
        self.ax.grid(True, linestyle='--', alpha=0.7)
        self.ax.legend(fontsize=8)

        # Format x-axis for dates
        self.ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M:%S'))
        self.ax.xaxis.set_major_locator(mdates.AutoDateLocator())

        self.fig.autofmt_xdate()  # Auto-format date labels

        self.fig.tight_layout()

        # Embed the chart into Tkinter
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.master)
        self.canvas.draw()
        self.canvas.get_tk_widget().pack(fill='both', expand=True)

        # Initialize data containers
        self.x_data = []
        self.y_data = []

        # Add interactive tooltips
        self.add_tooltips()

    def update_chart(self, x, y):
        """
        Update the chart with new data.

        :param x: List of datetime objects for the x-axis.
        :param y: List of values for the y-axis.
        """
        self.x_data = x
        self.y_data = y

        self.line.set_data(self.x_data, self.y_data)
        self.ax.relim()
        self.ax.autoscale_view()

        # Update the x-axis limits to fit the data
        if self.x_data:
            self.ax.set_xlim(min(self.x_data), max(self.x_data))

        self.canvas.draw()

    def add_tooltips(self):
        """
        Add interactive tooltips for the line chart.
        """
        cursor = mplcursors.cursor(self.line, hover=True)

        @cursor.connect("add")
        def on_add(sel):
            try:
                # Ensure sel.index is an integer
                index = int(sel.index)

                # Prevent IndexError if index is out of bounds
                if index >= len(self.x_data) or index < 0:
                    return

                time_str = self.x_data[index].strftime('%H:%M:%S')
                value = self.y_data[index]
                sel.annotation.set(text=f"Time: {time_str}\nValue: {value} {self.unit}")
                sel.annotation.get_bbox_patch().set(fc="white")
            except Exception as e:
                print(f"Error handling tooltip: {e}")

class RealtimeChartFrame(ctk.CTkFrame):
    def __init__(self, parent, db_path='database1.db'):
        super().__init__(parent)
        self.parent = parent
        self.pack(fill="both", expand=True)

        # Define parameters to measure, their units, and colors
        self.parameters = {
            'Temperature': {'unit': '°C', 'color': 'red'},
            'Humidity': {'unit': '%RH', 'color': 'blue'},
            'Dust Sensor': {'unit': 'µg/m³', 'color': 'green'},
            'CO': {'unit': 'ppm', 'color': 'purple'},
            'NH3': {'unit': 'ppm', 'color': 'orange'},
            'Air Pressure': {'unit': 'hPa', 'color': 'brown'},
        }

        # Mapping from parameter names to database column names
        self.param_to_db_column = {
            'Temperature': 'temp',
            'Humidity': 'humi',
            'Dust Sensor': 'pm25',
            'CO': 'co',
            'NH3': 'nh3',
            'Air Pressure': 'airpresser',
        }

        # Initialize selected date to today (not used in this version)
        self.selected_date = datetime.now().date()

        # Current database path
        self.db_path = db_path

        # Connect to the SQLite database
        self.connect_to_db()

        # Create a scrollable frame to contain all the charts
        self.main_frame = ctk.CTkScrollableFrame(
            self,
            width=1400,
            height=800,
            border_width=2,
            border_color="grey",
            corner_radius=10
        )
        self.main_frame.pack(padx=20, pady=20, fill="both", expand=True)

        # Configure grid layout within the scrollable frame
        for i in range(3):
            self.main_frame.grid_rowconfigure(i, weight=1, pad=10)
        for j in range(2):
            self.main_frame.grid_columnconfigure(j, weight=1, pad=10)

        # Position of each parameter in the grid
        self.grid_positions = {
            'Temperature': (0, 0),
            'Humidity': (0, 1),
            'Dust Sensor': (1, 0),
            'CO': (1, 1),
            'NH3': (2, 0),
            'Air Pressure': (2, 1),
        }

        # Create chart instances for each parameter
        self.charts = {}
        self.create_charts()

        # Start the periodic data update
        self.update_charts()

    def connect_to_db(self):
        """
        Establish a connection to the SQLite database.
        """
        try:
            if hasattr(self, 'conn') and self.conn:
                self.conn.close()
            self.conn = sqlite3.connect(self.db_path, check_same_thread=False)
            self.cursor = self.conn.cursor()
            print(f"Connected to {self.db_path} successfully.")
        except sqlite3.Error as e:
            print(f"Error connecting to database {self.db_path}: {e}")
            self.conn = None
            self.cursor = None

    def set_database(self, db_path):
        """
        Change the current database to a new one.

        :param db_path: Path to the new SQLite database.
        """
        if self.db_path == db_path:
            return  # No change needed

        self.db_path = db_path
        self.connect_to_db()
        # Immediately fetch and update charts with new data
        self.refresh_charts()

    def fetch_latest_data(self):
        """
        Fetch the latest 20 entries from the database.

        :return: DataFrame containing the latest 20 entries.
        """
        if not self.conn or not self.cursor:
            print("Database connection is not established.")
            return pd.DataFrame()

        try:
            # Fetch the latest 20 entries ordered by date_save and time_save descending
            # Convert date_save from dd/mm/yyyy to yyyy-mm-dd for proper sorting
            # query = """
            #     SELECT date_save, time_save, temp, humi, co, pm25, nh3, airpresser
            #     FROM measurements
            #     ORDER BY
            #         substr(date_save,7,4) || '-' || substr(date_save,4,2) || '-' || substr(date_save,1,2) DESC,
            #         time_save DESC
            #     LIMIT 20
            # """
            query = """
                SELECT date_save, time_save, temp, humi, co, pm25, nh3, airpresser
                FROM measurements
                ORDER BY ROWID DESC
                LIMIT 20
            """
            self.cursor.execute(query)
            rows = self.cursor.fetchall()

            if not rows:
                print("No data found in the database.")
                return pd.DataFrame()

            # Create DataFrame
            df = pd.DataFrame(rows, columns=['date_save', 'time_save', 'temp', 'humi', 'co', 'pm25', 'nh3', 'airpresser'])

            # Combine date and time into a single datetime column
            df['timestamp'] = pd.to_datetime(df['date_save'] + ' ' + df['time_save'], format='%d/%m/%Y %H:%M:%S')

            # Sort by timestamp ascending for proper plotting
            df = df.sort_values('timestamp')

            return df

        except sqlite3.Error as e:
            print(f"Error fetching data from database {self.db_path}: {e}")
            return pd.DataFrame()

    def process_data(self, df):
        """
        Process the fetched data.

        :param df: DataFrame containing the latest 20 entries.
        :return: Dictionary of DataFrames for each parameter.
        """
        if df.empty:
            return {param: pd.DataFrame() for param in self.parameters}

        data = {}
        for param, info in self.parameters.items():
            column = self.param_to_db_column[param]
            data[param] = df[['timestamp', column]].dropna()

        return data

    def create_charts(self):
        """
        Create chart instances and embed them into the grid.
        """
        for param, info in self.parameters.items():
            position = self.grid_positions[param]
            frame = ctk.CTkFrame(self.main_frame, width=500, height=400)
            frame.grid(row=position[0], column=position[1], padx=10, pady=10, sticky="nsew")
            frame.grid_propagate(False)  # Prevent frame from resizing based on content

            # Initialize ParameterChart without data
            chart = ParameterChart(
                master=frame,
                param_name=param,
                unit=info['unit'],
                color=info['color']
            )
            self.charts[param] = chart

    def update_charts(self):
        """
        Fetch new data and update all charts. Schedule the next update.
        """
        df = self.fetch_latest_data()
        processed_data = self.process_data(df)

        for param, chart in self.charts.items():
            data = processed_data.get(param)
            if data is not None and not data.empty:
                x = data['timestamp'].tolist()
                y = data[self.param_to_db_column[param]].tolist()
                chart.update_chart(x, y)
            else:
                # If no data, clear the chart
                chart.update_chart([], [])

        # Schedule the next update after 10000 ms (10 seconds)
        self.after(10000, self.update_charts)

    def refresh_charts(self):
        """
        Refresh the charts immediately after a database change.
        """
        df = self.fetch_latest_data()
        processed_data = self.process_data(df)

        for param, chart in self.charts.items():
            data = processed_data.get(param)
            if data is not None and not data.empty:
                x = data['timestamp'].tolist()
                y = data[self.param_to_db_column[param]].tolist()
                chart.update_chart(x, y)
            else:
                # If no data, clear the chart
                chart.update_chart([], [])

    def export_to_excel(self):
        """
        Export the currently displayed data to an Excel file.
        """
        # Lấy tất cả các bản ghi hiện tại trong Treeview
        records = []
        for row_id in self.tree.get_children():
            row = self.tree.item(row_id)['values']
            records.append(row)

        if not records:
            messagebox.showwarning("Cảnh Báo", "Không có dữ liệu để xuất.")
            return

        # Tạo DataFrame từ dữ liệu
        df = pd.DataFrame(records, columns=["Ngày", "Giờ", "Nhiệt Độ", "Độ Ẩm", "CO", "PM2.5", "NH3", "Áp Suất Không Khí"])

        # Mở hộp thoại để chọn nơi lưu tệp Excel
        file_path = filedialog.asksaveasfilename(
            defaultextension=".xlsx",
            filetypes=[("Excel files", "*.xlsx"), ("All files", "*.*")],
            title="Lưu Dữ Liệu Thành Excel"
        )
        if not file_path:
            return  # Người dùng đã hủy bỏ hộp thoại

        try:
            # Lưu DataFrame vào tệp Excel
            df.to_excel(file_path, index=False)
            messagebox.showinfo("Thành Công", f"Dữ liệu đã được lưu thành công tại:\n{file_path}")
        except Exception as e:
            messagebox.showerror("Lỗi", f"Đã xảy ra lỗi khi lưu tệp Excel:\n{e}")

    def on_closing(self):
        """
        Handle the closing of the frame/application.
        """
        if hasattr(self, 'conn') and self.conn:
            self.conn.close()
            print(f"Database connection to {self.db_path} closed.")
        self.destroy()

# Thiết lập giao diện
ctk.set_appearance_mode("System")  # Có thể chọn "System", "Dark", "Light"
ctk.set_default_color_theme("blue")  # Có thể chọn "blue", "green", "dark-blue"

class MainApp(ctk.CTk):
    def __init__(self):
        super().__init__()
        self.title("Chương Trình Chính")
        self.geometry("1500x900")  # Kích thước tùy chỉnh cho chương trình chính

        # Tạo một Frame để chứa RealtimeChartFrame và các Radiobuttons
        main_frame = ctk.CTkFrame(self)
        main_frame.pack(padx=20, pady=20, fill="both", expand=True)
        self.pack_propagate(False)
        # Tạo và nhúng RealtimeChartFrame vào main_frame
        self.realtime_chart = RealtimeChartFrame(main_frame)
        self.realtime_chart.pack(fill="both", expand=True)

        # Thêm các widget khác của chương trình chính nếu cần
        # Ví dụ: Một menu, sidebar, etc.
        self.create_menu()
        self.create_database_radiobuttons()

    def create_menu(self):
        """
        Tạo menu cho chương trình chính.
        """
        # Tạo một menu bar
        menu_bar = ctk.CTkOptionMenu(
            self,
            values=["Tùy chọn 1", "Tùy chọn 2"],
            command=self.menu_callback,
            width=150,
            height=30
        )
        menu_bar.pack(pady=10)

    def create_database_radiobuttons(self):
        """
        Tạo ba Radiobuttons để chọn cơ sở dữ liệu.
        """
        # Frame chứa các Radiobuttons
        radio_frame = ctk.CTkFrame(self)
        radio_frame.pack(pady=10)

        # Biến để lưu giá trị chọn
        self.db_choice = ctk.StringVar(value="database1.db")

        # Danh sách các cơ sở dữ liệu
        databases = [("Cơ Sở Dữ Liệu 1", "database1.db"),
                     ("Cơ Sở Dữ Liệu 2", "database2.db"),
                     ("Cơ Sở Dữ Liệu 3", "database3.db")]

        # Tạo các Radiobuttons
        for text, db in databases:
            radio = ctk.CTkRadioButton(
                radio_frame,
                text=text,
                variable=self.db_choice,
                value=db,
                command=self.on_db_change
            )
            radio.pack(side="left", padx=10)

    def on_db_change(self):
        """
        Xử lý khi chọn một Radiobutton để thay đổi cơ sở dữ liệu.
        """
        selected_db = self.db_choice.get()
        print(f"Selected database: {selected_db}")
        self.realtime_chart.set_database(selected_db)

    def menu_callback(self, choice):
        """
        Xử lý sự kiện khi một tùy chọn trong menu được chọn.

        :param choice: Tùy chọn được chọn.
        """
        messagebox.showinfo("Menu", f"Bạn đã chọn: {choice}")

    def on_closing(self):
        """
        Xử lý khi đóng ứng dụng chính.
        """
        # Đóng kết nối cơ sở dữ liệu nếu cần
        try:
            self.realtime_chart.on_closing()
        except:
            pass
        self.destroy()

if __name__ == "__main__":
    app = MainApp()
    app.protocol("WM_DELETE_WINDOW", app.on_closing)
    app.mainloop()
