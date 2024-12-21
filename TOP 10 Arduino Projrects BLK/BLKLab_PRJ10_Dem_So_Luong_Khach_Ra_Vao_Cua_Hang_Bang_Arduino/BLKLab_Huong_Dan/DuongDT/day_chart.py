# # import customtkinter as ctk
# # import pandas as pd
# # import matplotlib.pyplot as plt
# # from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
# # import sqlite3  # Import sqlite3 for database operations
# # from datetime import datetime, timedelta
# # import mplcursors
# # from tkcalendar import DateEntry  # Import DateEntry for date selection
# #
# #
# # class ParameterChart:
# #     def __init__(self, master, summary, param_name, unit, color):
# #         """
# #         Initialize a chart for a specific parameter.
# #
# #         :param master: Frame containing the chart.
# #         :param summary: DataFrame summarizing data for the chart.
# #         :param param_name: Name of the parameter.
# #         :param unit: Unit of the parameter.
# #         :param color: Color of the bar chart.
# #         """
# #         self.master = master
# #         self.summary = summary
# #         self.param_name = param_name
# #         self.unit = unit
# #         self.color = color
# #
# #         self.create_chart()
# #
# #     def create_chart(self):
# #         """
# #         Create and plot the bar chart along with the average line.
# #         """
# #         # Set figsize corresponding to the frame size (width, height) in inches
# #         figsize = (6, 4)  # Increased size for better label alignment
# #         self.fig, self.ax = plt.subplots(figsize=figsize, dpi=100)
# #
# #         # Plot the bar chart with limits from min_value to max_value
# #         self.bars = self.ax.bar(
# #             self.summary['hour'],
# #             self.summary['max_value'] - self.summary['min_value'],
# #             bottom=self.summary['min_value'],
# #             color=self.color,
# #             edgecolor='black',
# #             alpha=0.7,
# #             label='Phạm vi'
# #         )
# #
# #         # Plot the line connecting average values
# #         self.line, = self.ax.plot(
# #             self.summary['hour'],
# #             self.summary['avg_value'],
# #             color='black',
# #             marker='o',
# #             linestyle='-',
# #             linewidth=2,
# #             label='Trung bình'
# #         )
# #
# #         # Set title and axis labels
# #         self.ax.set_title(f'{self.param_name} ({self.unit})', fontsize=12, weight='bold')
# #         self.ax.set_xlabel('Giờ', fontsize=10)
# #         self.ax.set_ylabel(self.unit, fontsize=10)
# #         self.ax.set_xticks(self.summary['hour'])
# #         self.ax.set_xticklabels(
# #             [f"{hour}" for hour in self.summary['hour']],
# #             rotation=45,
# #             ha='center'  # Changed alignment to 'center' for better alignment with bars
# #         )
# #         self.ax.grid(axis='y', linestyle='--', alpha=0.7)
# #         self.ax.legend(fontsize=8)
# #
# #         # Adjust layout to prevent clipping of tick-labels
# #         self.fig.tight_layout()
# #
# #         # Add interactive tooltips
# #         self.add_tooltips()
# #
# #         # Embed the chart into Tkinter
# #         self.canvas = FigureCanvasTkAgg(self.fig, master=self.master)
# #         self.canvas.draw()
# #         self.canvas.get_tk_widget().pack(fill='both', expand=True)
# #
# #     def add_tooltips(self):
# #         """
# #         Add interactive tooltips for bars and the average line.
# #         """
# #         # Tooltip for bars
# #         cursor_bars = mplcursors.cursor(self.bars, hover=True)
# #
# #         @cursor_bars.connect("add")
# #         def on_add_bars(sel):
# #             try:
# #                 # Determine the hour from the bar's center x-value
# #                 bar = sel.artist[sel.index]
# #                 hour = int(bar.get_x() + bar.get_width() / 2)
# #                 # Filter DataFrame to get the row corresponding to that hour
# #                 row = self.summary[self.summary['hour'] == hour].iloc[0]
# #                 text = (
# #                     f"Giá trị cao nhất: {row['max_value']} {self.unit} tại {row['max_time']}\n"
# #                     f"Giá trị thấp nhất: {row['min_value']} {self.unit} tại {row['min_time']}\n"
# #                     f"Giá trị trung bình: {row['avg_value']} {self.unit}"
# #                 )
# #                 sel.annotation.set(text=text)
# #                 sel.annotation.get_bbox_patch().set(fc="white")
# #             except Exception as e:
# #                 print(f"Lỗi khi xử lý tooltip cho cột: {e}")
# #
# #         # Tooltip for the average line
# #         # cursor_line = mplcursors.cursor(self.line, hover=True)
# #         #
# #         # @cursor_line.connect("add")
# #         # def on_add_line(sel):
# #         #     try:
# #         #         # Get x and y values from the selected point
# #         #         x, y = sel.target  # sel.target is a tuple (x, y)
# #         #         hour = int(round(x))  # Round to get the nearest hour
# #         #         # Filter DataFrame to get the row corresponding to that hour
# #         #         row = self.summary[self.summary['hour'] == hour].iloc[0]
# #         #         text = (
# #         #             f"Giờ: {row['hour']:02d}:00\n"
# #         #             f"Trung bình: {row['avg_value']} {self.unit}\n"
# #         #             f"Cao nhất: {row['max_value']} {self.unit} tại {row['max_time']}\n"
# #         #             f"Thấp nhất: {row['min_value']} {self.unit} tại {row['min_time']}"
# #         #         )
# #         #         sel.annotation.set(text=text)
# #         #         sel.annotation.get_bbox_patch().set(fc="white")
# #         #     except Exception as e:
# #         #         print(f"Lỗi khi xử lý tooltip cho đường line: {e}")
# #
# #
# # class EnvironmentChartApp:
# #     def __init__(self, root):
# #         """
# #         Initialize the environment parameters chart application.
# #
# #         :param root: Tkinter root window.
# #         """
# #         self.root = root
# #         self.root.title("Biểu đồ Các Thông Số Môi Trường 24 Giờ")
# #         self.root.geometry("1300x1000")
# #         self.root.resizable(False, False)  # Prevent window resizing (optional)
# #
# #         # Define parameters to measure, their units, ranges, and colors
# #         self.parameters = {
# #             'Temperature': {'unit': '°C', 'range': (15, 35), 'color': 'red'},
# #             'Humidity': {'unit': '%RH', 'range': (30, 90), 'color': 'blue'},
# #             'Dust Sensor': {'unit': 'µg/m³', 'range': (0, 500), 'color': 'green'},
# #             'CO': {'unit': 'ppm', 'range': (0, 10), 'color': 'purple'},
# #             'NH3': {'unit': 'ppm', 'range': (0, 5), 'color': 'orange'},
# #             'Air Pressure': {'unit': 'hPa', 'range': (980, 1050), 'color': 'brown'},
# #         }
# #
# #         # Mapping from parameter names to database column names
# #         self.param_to_db_column = {
# #             'Temperature': 'temp',
# #             'Humidity': 'humi',
# #             'Dust Sensor': 'pm25',
# #             'CO': 'co',
# #             'NH3': 'nh3',
# #             'Air Pressure': 'airpresser',
# #         }
# #
# #         # Create data structures to store DataFrames and summaries for each parameter
# #         self.data_frames = {}
# #         self.summaries = {}
# #
# #         # Position of each parameter in the grid
# #         self.grid_positions = {
# #             'Temperature': (0, 0),
# #             'Humidity': (0, 1),
# #             'Dust Sensor': (1, 0),
# #             'CO': (1, 1),
# #             'NH3': (2, 0),
# #             'Air Pressure': (2, 1),
# #         }
# #
# #         # Initialize selected date to today
# #         self.selected_date = datetime.now().date()
# #
# #         # Connect to the SQLite database
# #         self.connect_to_db()
# #
# #         # Create a control frame above the main_frame for date selection
# #         self.control_frame = ctk.CTkFrame(self.root)
# #         self.control_frame.pack(padx=20, pady=10, fill='x')
# #
# #         # Label to display the selected date in dd/mm/yyyy format
# #         self.selected_date_label = ctk.CTkLabel(
# #             self.control_frame,
# #             text=f"Selected Date: {self.selected_date.strftime('%d/%m/%Y')}",
# #             font=ctk.CTkFont(size=14, weight="bold")
# #         )
# #         self.selected_date_label.pack(side='left', padx=10)
# #
# #         # Button to open date selection window
# #         self.select_date_button = ctk.CTkButton(
# #             self.control_frame,
# #             text="Select Date",
# #             command=self.open_date_selector
# #         )
# #         self.select_date_button.pack(side='left', padx=10)
# #
# #         # Create a scrollable frame to contain all the charts
# #         self.main_frame = ctk.CTkScrollableFrame(
# #             self.root,
# #             width=1050,
# #             height=600,
# #             border_width=2,
# #             border_color="grey",
# #             corner_radius=10
# #         )
# #         self.main_frame.pack(padx=20, pady=20)
# #
# #         # Configure grid layout within the scrollable frame
# #         for i in range(3):
# #             self.main_frame.grid_rowconfigure(i, weight=1, pad=10)
# #         for j in range(2):
# #             self.main_frame.grid_columnconfigure(j, weight=1, pad=10)
# #
# #         # Create and embed all charts
# #         self.create_charts()
# #
# #     def connect_to_db(self):
# #         """
# #         Establish a connection to the SQLite database.
# #         """
# #         try:
# #             self.conn = sqlite3.connect('database.db')
# #             self.cursor = self.conn.cursor()
# #             print("Connected to database.db successfully.")
# #         except sqlite3.Error as e:
# #             print(f"Error connecting to database: {e}")
# #             self.conn = None
# #             self.cursor = None
# #
# #     def open_date_selector(self):
# #         """
# #         Open a toplevel window to select a date.
# #         """
# #         # Create a new toplevel window
# #         self.top = ctk.CTkToplevel(self.root)
# #         self.top.title("Select Date")
# #         self.top.geometry("300x150")
# #         self.top.resizable(False, False)
# #
# #         # Instruction label
# #         instruction_label = ctk.CTkLabel(
# #             self.top,
# #             text="Please select a date:",
# #             font=ctk.CTkFont(size=12)
# #         )
# #         instruction_label.pack(pady=10)
# #
# #         # DateEntry widget from tkcalendar with dd/mm/yyyy format
# #         self.date_entry = DateEntry(
# #             self.top,
# #             width=12,
# #             background='darkblue',
# #             foreground='white',
# #             borderwidth=2,
# #             date_pattern='dd/mm/yyyy'  # Set date pattern to dd/mm/yyyy
# #         )
# #         self.date_entry.pack(pady=5)
# #
# #         # Confirm button
# #         confirm_button = ctk.CTkButton(
# #             self.top,
# #             text="Confirm",
# #             command=self.confirm_date_selection
# #         )
# #         confirm_button.pack(pady=10)
# #
# #     def confirm_date_selection(self):
# #         """
# #         Confirm the selected date and update the label.
# #         """
# #         selected_date_str = self.date_entry.get()
# #         try:
# #             # Convert the selected date string to a date object using dd/mm/yyyy format
# #             self.selected_date = datetime.strptime(selected_date_str, '%d/%m/%Y').date()
# #             # Update the label with the selected date in dd/mm/yyyy format
# #             self.selected_date_label.configure(text=f"Selected Date: {self.selected_date.strftime('%d/%m/%Y')}")
# #             # Close the toplevel window
# #             self.top.destroy()
# #             # Refresh the charts based on the selected date
# #             self.refresh_charts()
# #         except ValueError as ve:
# #             print(f"Invalid date format: {ve}")
# #             # Optionally, display an error message to the user
# #             error_popup = ctk.CTkToplevel(self.root)
# #             error_popup.title("Invalid Date")
# #             error_popup.geometry("250x100")
# #             error_label = ctk.CTkLabel(
# #                 error_popup,
# #                 text="Invalid date format. Please select a valid date.",
# #                 wraplength=200
# #             )
# #             error_label.pack(pady=20)
# #             ok_button = ctk.CTkButton(
# #                 error_popup,
# #                 text="OK",
# #                 command=error_popup.destroy
# #             )
# #             ok_button.pack(pady=10)
# #
# #     def refresh_charts(self):
# #         """
# #         Refresh all charts based on the selected date.
# #         """
# #         # Clear existing charts
# #         for widget in self.main_frame.winfo_children():
# #             widget.destroy()
# #
# #         # Recreate charts with the new date
# #         self.create_charts()
# #
# #     def fetch_data_from_db(self, param_name, param_info):
# #         """
# #         Fetch data from the database for a specific parameter and date.
# #
# #         :param param_name: Name of the parameter.
# #         :param param_info: Information about the parameter (unit, range, color).
# #         :return: DataFrame containing the fetched data.
# #         """
# #         if not self.conn or not self.cursor:
# #             print("Database connection is not established.")
# #             return pd.DataFrame()
# #
# #         # Format the selected date as dd/mm/yyyy to match the database format
# #         selected_date_str = self.selected_date.strftime('%d/%m/%Y')
# #
# #         try:
# #             # Determine the correct column name based on the parameter
# #             db_column = self.param_to_db_column.get(param_name)
# #             if not db_column:
# #                 print(f"No mapping found for parameter: {param_name}")
# #                 return pd.DataFrame()
# #
# #             # Fetch records for the selected date
# #             query = f"""
# #                 SELECT time_save, {db_column} FROM measurements
# #                 WHERE date_save = ?
# #             """
# #
# #             self.cursor.execute(query, (selected_date_str,))
# #             rows = self.cursor.fetchall()
# #
# #             # If no data is found, return an empty DataFrame
# #             if not rows:
# #                 print(f"No data found for {param_name} on {selected_date_str}.")
# #                 return pd.DataFrame()
# #
# #             # Create a DataFrame from the fetched data
# #             df = pd.DataFrame(rows, columns=['time_save', db_column])
# #
# #             # Combine date and time into a single datetime column
# #             df['timestamp'] = pd.to_datetime(
# #                 self.selected_date.strftime('%d/%m/%Y') + ' ' + df['time_save'],
# #                 format='%d/%m/%Y %H:%M:%S'
# #             )
# #
# #             # Extract the hour from the timestamp
# #             df['hour'] = df['timestamp'].dt.hour
# #
# #             return df
# #         except sqlite3.Error as e:
# #             print(f"Error fetching data from database: {e}")
# #             return pd.DataFrame()
# #
# #     def process_data(self, df, param_name):
# #         """
# #         Process data to calculate min, max, avg, and corresponding times for each hour.
# #
# #         :param df: DataFrame containing data for the parameter.
# #         :param param_name: Name of the parameter.
# #         :return: Summary DataFrame.
# #         """
# #         if df.empty:
# #             # Return an empty summary if there's no data
# #             return pd.DataFrame()
# #
# #         grouped = df.groupby('hour')
# #
# #         db_column = self.param_to_db_column.get(param_name)
# #         if not db_column:
# #             print(f"No mapping found for parameter: {param_name}")
# #             return pd.DataFrame()
# #
# #         summary = grouped.agg(
# #             min_value=(db_column, 'min'),
# #             max_value=(db_column, 'max'),
# #             avg_value=(db_column, 'mean')
# #         ).reset_index()
# #
# #         # Find the times corresponding to min and max values
# #         min_times = grouped.apply(
# #             lambda x: x.loc[x[db_column].idxmin()]['timestamp'].strftime('%H:%M:%S')
# #         )
# #         max_times = grouped.apply(
# #             lambda x: x.loc[x[db_column].idxmax()]['timestamp'].strftime('%H:%M:%S')
# #         )
# #
# #         summary['min_time'] = min_times.values
# #         summary['max_time'] = max_times.values
# #         summary['avg_value'] = summary['avg_value'].round(2)
# #
# #         return summary
# #
# #     def create_charts(self):
# #         """
# #         Create and display all charts in the 3x2 grid.
# #         """
# #         for param, info in self.parameters.items():
# #             # Fetch data from the database for the parameter
# #             df = self.fetch_data_from_db(param, info)
# #
# #             if df.empty:
# #                 # If no data is found, display a message instead of a chart
# #                 position = self.grid_positions[param]
# #                 frame = ctk.CTkFrame(self.main_frame, width=500, height=400)
# #                 frame.grid(row=position[0], column=position[1], padx=10, pady=10, sticky="nsew")
# #                 frame.grid_propagate(False)  # Prevent frame from resizing based on content
# #
# #                 no_data_label = ctk.CTkLabel(
# #                     frame,
# #                     text=f"No data available for {param} on {self.selected_date.strftime('%d/%m/%Y')}",
# #                     wraplength=480,
# #                     text_color="red",
# #                     font=ctk.CTkFont(size=12, weight="bold")
# #                 )
# #                 no_data_label.pack(expand=True)
# #                 continue
# #
# #             # Process data to get summary statistics
# #             summary = self.process_data(df, param)
# #             self.summaries[param] = summary
# #
# #             # Create a frame for the chart
# #             position = self.grid_positions[param]
# #             frame = ctk.CTkFrame(self.main_frame, width=500, height=400)
# #             frame.grid(row=position[0], column=position[1], padx=10, pady=10, sticky="nsew")
# #             frame.grid_propagate(False)  # Prevent frame from resizing based on content
# #
# #             # Create and embed the chart
# #             ParameterChart(
# #                 master=frame,
# #                 summary=summary,
# #                 param_name=param,
# #                 unit=info['unit'],
# #                 color=info['color']
# #             )
# #
# #     def run(self):
# #         """
# #         Start the main loop of the application.
# #         """
# #         self.root.mainloop()
# #         # Close the database connection when the application is closed
# #         if self.conn:
# #             self.conn.close()
# #             print("Database connection closed.")
# #
# #
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
# # # từ chương trình này, hãy sửa lại chương trình lọc theo năm, trục x sẽ là 12 tháng từ 1-12
#
#
# import customtkinter as ctk
# from tkinter import ttk, messagebox, filedialog
# import sqlite3
# from datetime import datetime, timedelta
# import pandas as pd
# import matplotlib.pyplot as plt
# from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
# import mplcursors
# from tkcalendar import DateEntry
#
#
# class ParameterChart:
#     def __init__(self, master, summary, param_name, unit, color):
#         """
#         Initialize a chart for a specific parameter.
#
#         :param master: Frame containing the chart.
#         :param summary: DataFrame summarizing data for the chart.
#         :param param_name: Name of the parameter.
#         :param unit: Unit of the parameter.
#         :param color: Color of the bar chart.
#         """
#         self.master = master
#         self.summary = summary
#         self.param_name = param_name
#         self.unit = unit
#         self.color = color
#
#         self.create_chart()
#
#     def create_chart(self):
#         """
#         Create and plot the bar chart along with the average line.
#         """
#         # Set figsize corresponding to the frame size (width, height) in inches
#         figsize = (6, 4)  # Increased size for better label alignment
#         self.fig, self.ax = plt.subplots(figsize=figsize, dpi=100)
#
#         # Plot the bar chart with limits from min_value to max_value
#         self.bars = self.ax.bar(
#             self.summary['hour'],
#             self.summary['max_value'] - self.summary['min_value'],
#             bottom=self.summary['min_value'],
#             color=self.color,
#             edgecolor='black',
#             alpha=0.7,
#             label='Phạm vi'
#         )
#
#         # Plot the line connecting average values
#         self.line, = self.ax.plot(
#             self.summary['hour'],
#             self.summary['avg_value'],
#             color='black',
#             marker='o',
#             linestyle='-',
#             linewidth=2,
#             label='Trung bình'
#         )
#
#         # Set title and axis labels
#         self.ax.set_title(f'{self.param_name} ({self.unit})', fontsize=12, weight='bold')
#         self.ax.set_xlabel('Giờ', fontsize=10)
#         self.ax.set_ylabel(self.unit, fontsize=10)
#         self.ax.set_xticks(self.summary['hour'])
#         self.ax.set_xticklabels(
#             [f"{hour}" for hour in self.summary['hour']],
#             rotation=45,
#             ha='center'  # Changed alignment to 'center' for better alignment with bars
#         )
#         self.ax.grid(axis='y', linestyle='--', alpha=0.7)
#         self.ax.legend(fontsize=8)
#
#         # Adjust layout to prevent clipping of tick-labels
#         self.fig.tight_layout()
#
#         # Add interactive tooltips
#         self.add_tooltips()
#
#         # Embed the chart into Tkinter
#         self.canvas = FigureCanvasTkAgg(self.fig, master=self.master)
#         self.canvas.draw()
#         self.canvas.get_tk_widget().pack(fill='both', expand=True)
#
#     def add_tooltips(self):
#         """
#         Add interactive tooltips for bars and the average line.
#         """
#         # Tooltip for bars
#         cursor_bars = mplcursors.cursor(self.bars, hover=True)
#
#         @cursor_bars.connect("add")
#         def on_add_bars(sel):
#             try:
#                 # Determine the hour from the bar's center x-value
#                 bar = sel.artist[sel.index]
#                 hour = int(bar.get_x() + bar.get_width() / 2)
#                 # Filter DataFrame to get the row corresponding to that hour
#                 row = self.summary[self.summary['hour'] == hour].iloc[0]
#                 text = (
#                     f"Giá trị cao nhất: {row['max_value']} {self.unit} tại {row['max_time']}\n"
#                     f"Giá trị thấp nhất: {row['min_value']} {self.unit} tại {row['min_time']}\n"
#                     f"Giá trị trung bình: {row['avg_value']} {self.unit}"
#                 )
#                 sel.annotation.set(text=text)
#                 sel.annotation.get_bbox_patch().set(fc="white")
#             except Exception as e:
#                 print(f"Lỗi khi xử lý tooltip cho cột: {e}")
#
#         # Uncomment below to add tooltips for the average line if needed
#         # cursor_line = mplcursors.cursor(self.line, hover=True)
#         #
#         # @cursor_line.connect("add")
#         # def on_add_line(sel):
#         #     try:
#         #         # Get x and y values from the selected point
#         #         x, y = sel.target  # sel.target is a tuple (x, y)
#         #         hour = int(round(x))  # Round to get the nearest hour
#         #         # Filter DataFrame to get the row corresponding to that hour
#         #         row = self.summary[self.summary['hour'] == hour].iloc[0]
#         #         text = (
#         #             f"Giờ: {row['hour']:02d}:00\n"
#         #             f"Trung bình: {row['avg_value']} {self.unit}\n"
#         #             f"Cao nhất: {row['max_value']} {self.unit} tại {row['max_time']}\n"
#         #             f"Thấp nhất: {row['min_value']} {self.unit} tại {row['min_time']}"
#         #         )
#         #         sel.annotation.set(text=text)
#         #         sel.annotation.get_bbox_patch().set(fc="white")
#         #     except Exception as e:
#         #         print(f"Lỗi khi xử lý tooltip cho đường line: {e}")
#
#
# class DayChartFrame(ctk.CTkFrame):
#     def __init__(self, parent):
#         """
#         Initialize the environment parameters chart frame.
#
#         :param parent: Parent widget.
#         """
#         super().__init__(parent)
#         self.parent = parent
#
#         # Define parameters to measure, their units, ranges, and colors
#         self.parameters = {
#             'Temperature': {'unit': '°C', 'range': (15, 35), 'color': 'red'},
#             'Humidity': {'unit': '%RH', 'range': (30, 90), 'color': 'blue'},
#             'Dust Sensor': {'unit': 'µg/m³', 'range': (0, 500), 'color': 'green'},
#             'CO': {'unit': 'ppm', 'range': (0, 10), 'color': 'purple'},
#             'NH3': {'unit': 'ppm', 'range': (0, 5), 'color': 'orange'},
#             'Air Pressure': {'unit': 'hPa', 'range': (980, 1050), 'color': 'brown'},
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
#         # Create data structures to store DataFrames and summaries for each parameter
#         self.data_frames = {}
#         self.summaries = {}
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
#         # Initialize selected date to today
#         self.selected_date = datetime.now().date()
#
#         # Connect to the SQLite database
#         self.connect_to_db()
#
#         # Create a control frame above the main_frame for date selection
#         self.control_frame = ctk.CTkFrame(self)
#         self.control_frame.pack(padx=20, pady=10, fill='x')
#
#         # Label to display the selected date in dd/mm/yyyy format
#         self.selected_date_label = ctk.CTkLabel(
#             self.control_frame,
#             text=f"Selected Date: {self.selected_date.strftime('%d/%m/%Y')}",
#             font=ctk.CTkFont(size=14, weight="bold")
#         )
#         self.selected_date_label.pack(side='left', padx=10)
#
#         # Button to open date selection window
#         self.select_date_button = ctk.CTkButton(
#             self.control_frame,
#             text="Select Date",
#             command=self.open_date_selector
#         )
#         self.select_date_button.pack(side='left', padx=10)
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
#         # Create and embed all charts
#         self.create_charts()
#
#     def connect_to_db(self):
#         """
#         Establish a connection to the SQLite database.
#         """
#         try:
#             self.conn = sqlite3.connect('database.db')
#             self.cursor = self.conn.cursor()
#             print("Connected to database.db successfully.")
#         except sqlite3.Error as e:
#             print(f"Error connecting to database: {e}")
#             self.conn = None
#             self.cursor = None
#
#     def open_date_selector(self):
#         """
#         Open a toplevel window to select a date.
#         """
#         # Create a new toplevel window
#         self.top = ctk.CTkToplevel(self)
#         self.top.title("Select Date")
#         self.top.geometry("300x150")
#         self.top.resizable(False, False)
#
#         # Instruction label
#         instruction_label = ctk.CTkLabel(
#             self.top,
#             text="Please select a date:",
#             font=ctk.CTkFont(size=12)
#         )
#         instruction_label.pack(pady=10)
#
#         # DateEntry widget from tkcalendar with dd/mm/yyyy format
#         self.date_entry = DateEntry(
#             self.top,
#             width=12,
#             background='darkblue',
#             foreground='white',
#             borderwidth=2,
#             date_pattern='dd/mm/yyyy'  # Set date pattern to dd/mm/yyyy
#         )
#         self.date_entry.pack(pady=5)
#
#         # Confirm button
#         confirm_button = ctk.CTkButton(
#             self.top,
#             text="Confirm",
#             command=self.confirm_date_selection
#         )
#         confirm_button.pack(pady=10)
#
#     def confirm_date_selection(self):
#         """
#         Confirm the selected date and update the label.
#         """
#         selected_date_str = self.date_entry.get()
#         try:
#             # Convert the selected date string to a date object using dd/mm/yyyy format
#             self.selected_date = datetime.strptime(selected_date_str, '%d/%m/%Y').date()
#             # Update the label with the selected date in dd/mm/yyyy format
#             self.selected_date_label.configure(text=f"Selected Date: {self.selected_date.strftime('%d/%m/%Y')}")
#             # Close the toplevel window
#             self.top.destroy()
#             # Refresh the charts based on the selected date
#             self.refresh_charts()
#         except ValueError as ve:
#             print(f"Invalid date format: {ve}")
#             # Optionally, display an error message to the user
#             error_popup = ctk.CTkToplevel(self)
#             error_popup.title("Invalid Date")
#             error_popup.geometry("250x100")
#             error_label = ctk.CTkLabel(
#                 error_popup,
#                 text="Invalid date format. Please select a valid date.",
#                 wraplength=200
#             )
#             error_label.pack(pady=20)
#             ok_button = ctk.CTkButton(
#                 error_popup,
#                 text="OK",
#                 command=error_popup.destroy
#             )
#             ok_button.pack(pady=10)
#
#     def refresh_charts(self):
#         """
#         Refresh all charts based on the selected date.
#         """
#         # Clear existing charts
#         for widget in self.main_frame.winfo_children():
#             widget.destroy()
#
#         # Recreate charts with the new date
#         self.create_charts()
#
#     def fetch_data_from_db(self, param_name, param_info):
#         """
#         Fetch data from the database for a specific parameter and date.
#
#         :param param_name: Name of the parameter.
#         :param param_info: Information about the parameter (unit, range, color).
#         :return: DataFrame containing the fetched data.
#         """
#         if not self.conn or not self.cursor:
#             print("Database connection is not established.")
#             return pd.DataFrame()
#
#         # Format the selected date as dd/mm/yyyy to match the database format
#         selected_date_str = self.selected_date.strftime('%d/%m/%Y')
#
#         try:
#             # Determine the correct column name based on the parameter
#             db_column = self.param_to_db_column.get(param_name)
#             if not db_column:
#                 print(f"No mapping found for parameter: {param_name}")
#                 return pd.DataFrame()
#
#             # Fetch records for the selected date
#             query = f"""
#                 SELECT time_save, {db_column} FROM measurements
#                 WHERE date_save = ?
#             """
#
#             self.cursor.execute(query, (selected_date_str,))
#             rows = self.cursor.fetchall()
#
#             # If no data is found, return an empty DataFrame
#             if not rows:
#                 print(f"No data found for {param_name} on {selected_date_str}.")
#                 return pd.DataFrame()
#
#             # Create a DataFrame from the fetched data
#             df = pd.DataFrame(rows, columns=['time_save', db_column])
#
#             # Combine date and time into a single datetime column
#             df['timestamp'] = pd.to_datetime(
#                 self.selected_date.strftime('%d/%m/%Y') + ' ' + df['time_save'],
#                 format='%d/%m/%Y %H:%M:%S'
#             )
#
#             # Extract the hour from the timestamp
#             df['hour'] = df['timestamp'].dt.hour
#
#             return df
#         except sqlite3.Error as e:
#             print(f"Error fetching data from database: {e}")
#             return pd.DataFrame()
#
#     def process_data(self, df, param_name):
#         """
#         Process data to calculate min, max, avg, and corresponding times for each hour.
#
#         :param df: DataFrame containing data for the parameter.
#         :param param_name: Name of the parameter.
#         :return: Summary DataFrame.
#         """
#         if df.empty:
#             # Return an empty summary if there's no data
#             return pd.DataFrame()
#
#         grouped = df.groupby('hour')
#
#         db_column = self.param_to_db_column.get(param_name)
#         if not db_column:
#             print(f"No mapping found for parameter: {param_name}")
#             return pd.DataFrame()
#
#         summary = grouped.agg(
#             min_value=(db_column, 'min'),
#             max_value=(db_column, 'max'),
#             avg_value=(db_column, 'mean')
#         ).reset_index()
#
#         # Find the times corresponding to min and max values
#         min_times = grouped.apply(
#             lambda x: x.loc[x[db_column].idxmin()]['timestamp'].strftime('%H:%M:%S')
#         )
#         max_times = grouped.apply(
#             lambda x: x.loc[x[db_column].idxmax()]['timestamp'].strftime('%H:%M:%S')
#         )
#
#         summary['min_time'] = min_times.values
#         summary['max_time'] = max_times.values
#         summary['avg_value'] = summary['avg_value'].round(2)
#
#         return summary
#
#     def create_charts(self):
#         """
#         Create and display all charts in the 3x2 grid.
#         """
#         for param, info in self.parameters.items():
#             # Fetch data from the database for the parameter
#             df = self.fetch_data_from_db(param, info)
#
#             if df.empty:
#                 # If no data is found, display a message instead of a chart
#                 position = self.grid_positions[param]
#                 frame = ctk.CTkFrame(self.main_frame, width=500, height=400)
#                 frame.grid(row=position[0], column=position[1], padx=10, pady=10, sticky="nsew")
#                 frame.grid_propagate(False)  # Prevent frame from resizing based on content
#
#                 no_data_label = ctk.CTkLabel(
#                     frame,
#                     text=f"No data available for {param} on {self.selected_date.strftime('%d/%m/%Y')}",
#                     wraplength=480,
#                     text_color="red",
#                     font=ctk.CTkFont(size=12, weight="bold")
#                 )
#                 no_data_label.pack(expand=True)
#                 continue
#
#             # Process data to get summary statistics
#             summary = self.process_data(df, param)
#             self.summaries[param] = summary
#
#             # Create a frame for the chart
#             position = self.grid_positions[param]
#             frame = ctk.CTkFrame(self.main_frame, width=500, height=400)
#             frame.grid(row=position[0], column=position[1], padx=10, pady=10, sticky="nsew")
#             frame.grid_propagate(False)  # Prevent frame from resizing based on content
#
#             # Create and embed the chart
#             ParameterChart(
#                 master=frame,
#                 summary=summary,
#                 param_name=param,
#                 unit=info['unit'],
#                 color=info['color']
#             )
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
# # Thiết lập giao diện
# ctk.set_appearance_mode("System")  # Có thể chọn "System", "Dark", "Light"
# ctk.set_default_color_theme("blue")  # Có thể chọn "blue", "green", "dark-blue"
#
# class MainApp(ctk.CTk):
#     def __init__(self):
#         super().__init__()
#         self.title("Chương Trình Chính")
#         self.geometry("1400x1000")  # Kích thước tùy chỉnh cho chương trình chính
#
#         # Tạo một Frame để chứa EnvironmentChartFrame và các phần tử giao diện khác
#         main_frame = ctk.CTkFrame(self)
#         main_frame.pack(padx=20, pady=20, fill="both", expand=True)
#
#         # Tạo và nhúng EnvironmentChartFrame vào main_frame
#         self.environment_chart = DayChartFrame(main_frame)
#         self.environment_chart.pack(fill="both", expand=True)
#
#         # Thêm các widget khác của chương trình chính nếu cần
#         # Ví dụ: Một menu, sidebar, etc.
#         self.create_menu()
#
#     def create_menu(self):
#         """
#         Tạo menu cho chương trình chính.
#         """
#         # Tạo một menu bar đơn giản
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
#             self.environment_chart.conn.close()
#         except:
#             pass
#         self.destroy()
#
# if __name__ == "__main__":
#     app = MainApp()
#     app.protocol("WM_DELETE_WINDOW", app.on_closing)
#     app.mainloop()

#######################################################


import customtkinter as ctk
from tkinter import ttk, messagebox, filedialog
import sqlite3
from datetime import datetime, timedelta
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import mplcursors
from tkcalendar import DateEntry


class ParameterChart:
    def __init__(self, master, summary, param_name, unit, color):
        """
        Initialize a chart for a specific parameter.

        :param master: Frame containing the chart.
        :param summary: DataFrame summarizing data for the chart.
        :param param_name: Name of the parameter.
        :param unit: Unit of the parameter.
        :param color: Color of the bar chart.
        """
        self.master = master
        self.summary = summary
        self.param_name = param_name
        self.unit = unit
        self.color = color

        self.create_chart()

    def create_chart(self):
        """
        Create and plot the bar chart along with the average line.
        """
        # Set figsize corresponding to the frame size (width, height) in inches
        figsize = (6, 4)  # Increased size for better label alignment
        self.fig, self.ax = plt.subplots(figsize=figsize, dpi=100)

        # Plot the bar chart with limits from min_value to max_value
        self.bars = self.ax.bar(
            self.summary['hour'],
            self.summary['max_value'] - self.summary['min_value'],
            bottom=self.summary['min_value'],
            color=self.color,
            edgecolor='black',
            alpha=0.7,
            label='Phạm vi'
        )

        # Plot the line connecting average values
        self.line, = self.ax.plot(
            self.summary['hour'],
            self.summary['avg_value'],
            color='black',
            marker='o',
            linestyle='-',
            linewidth=2,
            label='Trung bình'
        )

        # Set title and axis labels
        self.ax.set_title(f'{self.param_name} ({self.unit})', fontsize=12, weight='bold')
        self.ax.set_xlabel('Giờ', fontsize=10)
        self.ax.set_ylabel(self.unit, fontsize=10)
        self.ax.set_xticks(self.summary['hour'])
        self.ax.set_xticklabels(
            [f"{hour}" for hour in self.summary['hour']],
            rotation=45,
            ha='center'  # Changed alignment to 'center' for better alignment with bars
        )
        self.ax.grid(axis='y', linestyle='--', alpha=0.7)
        self.ax.legend(fontsize=8)

        # Adjust layout to prevent clipping of tick-labels
        self.fig.tight_layout()

        # Add interactive tooltips
        self.add_tooltips()

        # Embed the chart into Tkinter
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.master)
        self.canvas.draw()
        self.canvas.get_tk_widget().pack(fill='both', expand=True)

    def add_tooltips(self):
        """
        Add interactive tooltips for bars and the average line.
        """
        # Tooltip for bars
        cursor_bars = mplcursors.cursor(self.bars, hover=True)

        @cursor_bars.connect("add")
        def on_add_bars(sel):
            try:
                # Determine the hour from the bar's center x-value
                bar = sel.artist[sel.index]
                hour = int(bar.get_x() + bar.get_width() / 2)
                # Filter DataFrame to get the row corresponding to that hour
                row = self.summary[self.summary['hour'] == hour].iloc[0]
                text = (
                    f"Giá trị cao nhất: {row['max_value']} {self.unit} tại {row['max_time']}\n"
                    f"Giá trị thấp nhất: {row['min_value']} {self.unit} tại {row['min_time']}\n"
                    f"Giá trị trung bình: {row['avg_value']} {self.unit}"
                )
                sel.annotation.set(text=text)
                sel.annotation.get_bbox_patch().set(fc="white")
            except Exception as e:
                print(f"Lỗi khi xử lý tooltip cho cột: {e}")

        # Uncomment below to add tooltips for the average line if needed
        # cursor_line = mplcursors.cursor(self.line, hover=True)
        #
        # @cursor_line.connect("add")
        # def on_add_line(sel):
        #     try:
        #         # Get x and y values from the selected point
        #         x, y = sel.target  # sel.target is a tuple (x, y)
        #         hour = int(round(x))  # Round to get the nearest hour
        #         # Filter DataFrame to get the row corresponding to that hour
        #         row = self.summary[self.summary['hour'] == hour].iloc[0]
        #         text = (
        #             f"Giờ: {row['hour']:02d}:00\n"
        #             f"Trung bình: {row['avg_value']} {self.unit}\n"
        #             f"Cao nhất: {row['max_value']} {self.unit} tại {row['max_time']}\n"
        #             f"Thấp nhất: {row['min_value']} {self.unit} tại {row['min_time']}"
        #         )
        #         sel.annotation.set(text=text)
        #         sel.annotation.get_bbox_patch().set(fc="white")
        #     except Exception as e:
        #         print(f"Lỗi khi xử lý tooltip cho đường line: {e}")


class DayChartFrame(ctk.CTkFrame):
    def __init__(self, parent, db_file='database1.db'):
        """
        Initialize the environment parameters chart frame.

        :param parent: Parent widget.
        :param db_file: SQLite database file.
        """
        super().__init__(parent)
        self.parent = parent
        self.db_file = db_file  # Store the database file

        # Define parameters to measure, their units, ranges, and colors
        self.parameters = {
            'Temperature': {'unit': '°C', 'range': (15, 35), 'color': 'red'},
            'Humidity': {'unit': '%RH', 'range': (30, 90), 'color': 'blue'},
            'Dust Sensor': {'unit': 'µg/m³', 'range': (0, 500), 'color': 'green'},
            'CO': {'unit': 'ppm', 'range': (0, 10), 'color': 'purple'},
            'NH3': {'unit': 'ppm', 'range': (0, 5), 'color': 'orange'},
            'Air Pressure': {'unit': 'hPa', 'range': (980, 1050), 'color': 'brown'},
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

        # Create data structures to store DataFrames and summaries for each parameter
        self.data_frames = {}
        self.summaries = {}

        # Position of each parameter in the grid
        self.grid_positions = {
            'Temperature': (0, 0),
            'Humidity': (0, 1),
            'Dust Sensor': (1, 0),
            'CO': (1, 1),
            'NH3': (2, 0),
            'Air Pressure': (2, 1),
        }

        # Initialize selected date to today
        self.selected_date = datetime.now().date()

        # Connect to the SQLite database
        self.connect_to_db()

        # Create a control frame above the main_frame for date selection
        self.control_frame = ctk.CTkFrame(self)
        self.control_frame.pack(padx=20, pady=10, fill='x')

        # Label to display the selected date in dd/mm/yyyy format
        self.selected_date_label = ctk.CTkLabel(
            self.control_frame,
            text=f"Selected Date: {self.selected_date.strftime('%d/%m/%Y')}",
            font=ctk.CTkFont(size=14, weight="bold")
        )
        self.selected_date_label.pack(side='left', padx=10)

        # Button to open date selection window
        self.select_date_button = ctk.CTkButton(
            self.control_frame,
            text="Select Date",
            command=self.open_date_selector
        )
        self.select_date_button.pack(side='left', padx=10)

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

        # Create and embed all charts
        self.create_charts()

    def connect_to_db(self):
        """
        Establish a connection to the SQLite database.
        """
        try:
            self.conn = sqlite3.connect(self.db_file)
            self.cursor = self.conn.cursor()
            print(f"Connected to {self.db_file} successfully.")
        except sqlite3.Error as e:
            print(f"Error connecting to database: {e}")
            self.conn = None
            self.cursor = None

    def open_date_selector(self):
        """
        Open a toplevel window to select a date.
        """
        # Create a new toplevel window
        self.top = ctk.CTkToplevel(self)
        self.top.title("Select Date")
        self.top.geometry("300x150")
        self.top.resizable(False, False)

        # Instruction label
        instruction_label = ctk.CTkLabel(
            self.top,
            text="Please select a date:",
            font=ctk.CTkFont(size=12)
        )
        instruction_label.pack(pady=10)

        # DateEntry widget from tkcalendar with dd/mm/yyyy format
        self.date_entry = DateEntry(
            self.top,
            width=12,
            background='darkblue',
            foreground='white',
            borderwidth=2,
            date_pattern='dd/mm/yyyy'  # Set date pattern to dd/mm/yyyy
        )
        self.date_entry.pack(pady=5)

        # Confirm button
        confirm_button = ctk.CTkButton(
            self.top,
            text="Confirm",
            command=self.confirm_date_selection
        )
        confirm_button.pack(pady=10)

    def confirm_date_selection(self):
        """
        Confirm the selected date and update the label.
        """
        selected_date_str = self.date_entry.get()
        try:
            # Convert the selected date string to a date object using dd/mm/yyyy format
            self.selected_date = datetime.strptime(selected_date_str, '%d/%m/%Y').date()
            # Update the label with the selected date in dd/mm/yyyy format
            self.selected_date_label.configure(text=f"Selected Date: {self.selected_date.strftime('%d/%m/%Y')}")
            # Close the toplevel window
            self.top.destroy()
            # Refresh the charts based on the selected date
            self.refresh_charts()
        except ValueError as ve:
            print(f"Invalid date format: {ve}")
            # Optionally, display an error message to the user
            error_popup = ctk.CTkToplevel(self)
            error_popup.title("Invalid Date")
            error_popup.geometry("250x100")
            error_label = ctk.CTkLabel(
                error_popup,
                text="Invalid date format. Please select a valid date.",
                wraplength=200
            )
            error_label.pack(pady=20)
            ok_button = ctk.CTkButton(
                error_popup,
                text="OK",
                command=error_popup.destroy
            )
            ok_button.pack(pady=10)

    def refresh_charts(self):
        """
        Refresh all charts based on the selected date.
        """
        # Clear existing charts
        for widget in self.main_frame.winfo_children():
            widget.destroy()

        # Recreate charts with the new date
        self.create_charts()

    def fetch_data_from_db(self, param_name, param_info):
        """
        Fetch data from the database for a specific parameter and date.

        :param param_name: Name of the parameter.
        :param param_info: Information about the parameter (unit, range, color).
        :return: DataFrame containing the fetched data.
        """
        if not self.conn or not self.cursor:
            print("Database connection is not established.")
            return pd.DataFrame()

        # Format the selected date as dd/mm/yyyy to match the database format
        selected_date_str = self.selected_date.strftime('%d/%m/%Y')

        try:
            # Determine the correct column name based on the parameter
            db_column = self.param_to_db_column.get(param_name)
            if not db_column:
                print(f"No mapping found for parameter: {param_name}")
                return pd.DataFrame()

            # Fetch records for the selected date
            query = f"""
                SELECT time_save, {db_column} FROM measurements
                WHERE date_save = ?
            """

            self.cursor.execute(query, (selected_date_str,))
            rows = self.cursor.fetchall()

            # If no data is found, return an empty DataFrame
            if not rows:
                print(f"No data found for {param_name} on {selected_date_str}.")
                return pd.DataFrame()

            # Create a DataFrame from the fetched data
            df = pd.DataFrame(rows, columns=['time_save', db_column])

            # Combine date and time into a single datetime column
            df['timestamp'] = pd.to_datetime(
                self.selected_date.strftime('%d/%m/%Y') + ' ' + df['time_save'],
                format='%d/%m/%Y %H:%M:%S'
            )

            # Extract the hour from the timestamp
            df['hour'] = df['timestamp'].dt.hour

            return df
        except sqlite3.Error as e:
            print(f"Error fetching data from database: {e}")
            return pd.DataFrame()

    def process_data(self, df, param_name):
        """
        Process data to calculate min, max, avg, and corresponding times for each hour.

        :param df: DataFrame containing data for the parameter.
        :param param_name: Name of the parameter.
        :return: Summary DataFrame.
        """
        if df.empty:
            # Return an empty summary if there's no data
            return pd.DataFrame()

        grouped = df.groupby('hour')

        db_column = self.param_to_db_column.get(param_name)
        if not db_column:
            print(f"No mapping found for parameter: {param_name}")
            return pd.DataFrame()

        summary = grouped.agg(
            min_value=(db_column, 'min'),
            max_value=(db_column, 'max'),
            avg_value=(db_column, 'mean')
        ).reset_index()

        # Find the times corresponding to min and max values
        min_times = grouped.apply(
            lambda x: x.loc[x[db_column].idxmin()]['timestamp'].strftime('%H:%M:%S')
        )
        max_times = grouped.apply(
            lambda x: x.loc[x[db_column].idxmax()]['timestamp'].strftime('%H:%M:%S')
        )

        summary['min_time'] = min_times.values
        summary['max_time'] = max_times.values
        summary['avg_value'] = summary['avg_value'].round(2)

        return summary

    def create_charts(self):
        """
        Create and display all charts in the 3x2 grid.
        """
        for param, info in self.parameters.items():
            # Fetch data from the database for the parameter
            df = self.fetch_data_from_db(param, info)

            if df.empty:
                # If no data is found, display a message instead of a chart
                position = self.grid_positions[param]
                frame = ctk.CTkFrame(self.main_frame, width=500, height=400)
                frame.grid(row=position[0], column=position[1], padx=10, pady=10, sticky="nsew")
                frame.grid_propagate(False)  # Prevent frame from resizing based on content

                no_data_label = ctk.CTkLabel(
                    frame,
                    text=f"No data available for {param} on {self.selected_date.strftime('%d/%m/%Y')}",
                    wraplength=480,
                    text_color="red",
                    font=ctk.CTkFont(size=12, weight="bold")
                )
                no_data_label.pack(expand=True)
                continue

            # Process data to get summary statistics
            summary = self.process_data(df, param)
            self.summaries[param] = summary

            # Create a frame for the chart
            position = self.grid_positions[param]
            frame = ctk.CTkFrame(self.main_frame, width=500, height=400)
            frame.grid(row=position[0], column=position[1], padx=10, pady=10, sticky="nsew")
            frame.grid_propagate(False)  # Prevent frame from resizing based on content

            # Create and embed the chart
            ParameterChart(
                master=frame,
                summary=summary,
                param_name=param,
                unit=info['unit'],
                color=info['color']
            )

    def set_database(self, db_file):
        """
        Set a new database file, reconnect, and refresh charts.

        :param db_file: Path to the new SQLite database file.
        """
        if self.conn:
            self.conn.close()
            print(f"Closed connection to {self.db_file}.")

        self.db_file = db_file
        self.connect_to_db()
        self.refresh_charts()

    # Remove run method since mainloop is handled by MainApp
    # def run(self):
    #     """
    #     Start the main loop of the application.
    #     """
    #     self.root.mainloop()
    #     # Close the database connection when the application is closed
    #     if self.conn:
    #         self.conn.close()
    #         print("Database connection closed.")


# Thiết lập giao diện
ctk.set_appearance_mode("System")  # Có thể chọn "System", "Dark", "Light"
ctk.set_default_color_theme("blue")  # Có thể chọn "blue", "green", "dark-blue"

class MainApp(ctk.CTk):
    def __init__(self):
        super().__init__()
        self.title("Chương Trình Chính")
        self.geometry("1400x1000")  # Kích thước tùy chỉnh cho chương trình chính

        # Create a Frame to contain database selection and charts
        main_frame = ctk.CTkFrame(self)
        main_frame.pack(padx=20, pady=20, fill="both", expand=True)

        # Create a frame for database selection radiobuttons
        db_selection_frame = ctk.CTkFrame(main_frame)
        db_selection_frame.pack(pady=10, fill='x')

        # Label for database selection
        db_label = ctk.CTkLabel(
            db_selection_frame,
            text="Select Database:",
            font=ctk.CTkFont(size=14, weight="bold")
        )
        db_label.pack(side='left', padx=10)

        # Variable to hold the selected database
        self.selected_db = ctk.StringVar(value='database1.db')  # Default selection

        # Create Radiobuttons for database selection
        db1_radio = ctk.CTkRadioButton(
            db_selection_frame,
            text="Database 1",
            variable=self.selected_db,
            value='database1.db',
            command=self.on_db_selection_changed
        )
        db1_radio.pack(side='left', padx=5)

        db2_radio = ctk.CTkRadioButton(
            db_selection_frame,
            text="Database 2",
            variable=self.selected_db,
            value='database2.db',
            command=self.on_db_selection_changed
        )
        db2_radio.pack(side='left', padx=5)

        db3_radio = ctk.CTkRadioButton(
            db_selection_frame,
            text="Database 3",
            variable=self.selected_db,
            value='database3.db',
            command=self.on_db_selection_changed
        )
        db3_radio.pack(side='left', padx=5)

        # Create and embed DayChartFrame into main_frame
        self.environment_chart = DayChartFrame(main_frame, db_file=self.selected_db.get())
        self.environment_chart.pack(fill="both", expand=True)

        # Add other widgets as needed
        # Example: A menu, sidebar, etc.
        self.create_menu()

    def create_menu(self):
        """
        Tạo menu cho chương trình chính.
        """
        # Tạo một menu bar đơn giản
        menu_bar = ctk.CTkOptionMenu(
            self,
            values=["Tùy chọn 1", "Tùy chọn 2"],
            command=self.menu_callback,
            width=150,
            height=30
        )
        menu_bar.pack(pady=10)

    def menu_callback(self, choice):
        """
        Xử lý sự kiện khi một tùy chọn trong menu được chọn.

        :param choice: Tùy chọn được chọn.
        """
        messagebox.showinfo("Menu", f"Bạn đã chọn: {choice}")

    def on_db_selection_changed(self):
        """
        Callback when a database Radiobutton is selected.
        """
        selected_db_file = self.selected_db.get()
        print(f"Selected database: {selected_db_file}")
        self.environment_chart.set_database(selected_db_file)

    def on_closing(self):
        """
        Xử lý khi đóng ứng dụng chính.
        """
        # Đóng kết nối cơ sở dữ liệu nếu cần
        try:
            self.environment_chart.conn.close()
        except:
            pass
        self.destroy()

if __name__ == "__main__":
    app = MainApp()
    app.protocol("WM_DELETE_WINDOW", app.on_closing)
    app.mainloop()
