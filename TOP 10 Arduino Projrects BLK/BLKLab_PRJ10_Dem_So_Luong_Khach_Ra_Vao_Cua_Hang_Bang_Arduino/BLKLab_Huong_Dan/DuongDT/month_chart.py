# # import customtkinter as ctk
# # import pandas as pd
# # import matplotlib.pyplot as plt
# # from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
# # import sqlite3  # Import sqlite3 for database operations
# # from datetime import datetime
# # import mplcursors
# # import sys
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
# #         if self.summary.empty:
# #             # Display a message if there's no data
# #             self.fig, self.ax = plt.subplots(figsize=(6, 4), dpi=100)
# #             self.ax.text(0.5, 0.5, 'No data available', horizontalalignment='center',
# #                          verticalalignment='center', transform=self.ax.transAxes, fontsize=12, color='red')
# #             self.ax.axis('off')
# #         else:
# #             # Set figsize corresponding to the frame size (width, height) in inches
# #             figsize = (6, 4)  # Increased size for better label alignment
# #             self.fig, self.ax = plt.subplots(figsize=figsize, dpi=100)
# #
# #             # Plot the bar chart with limits from min_value to max_value
# #             self.bars = self.ax.bar(
# #                 self.summary['month'],
# #                 self.summary['max_value'] - self.summary['min_value'],
# #                 bottom=self.summary['min_value'],
# #                 color=self.color,
# #                 edgecolor='black',
# #                 alpha=0.7,
# #                 label='Phạm vi'
# #             )
# #
# #             # Plot the line connecting average values
# #             self.line, = self.ax.plot(
# #                 self.summary['month'],
# #                 self.summary['avg_value'],
# #                 color='black',
# #                 marker='o',
# #                 linestyle='-',
# #                 linewidth=2,
# #                 label='Trung bình'
# #             )
# #
# #             # Set title and axis labels
# #             self.ax.set_title(f'{self.param_name} ({self.unit})', fontsize=12, weight='bold')
# #             self.ax.set_xlabel('Tháng', fontsize=10)
# #             self.ax.set_ylabel(self.unit, fontsize=10)
# #             self.ax.set_xticks(self.summary['month'])
# #             self.ax.set_xticklabels(
# #                 [f"{month}" for month in self.summary['month']],
# #                 rotation=45,
# #                 ha='center'  # Changed alignment to 'center' for better alignment with bars
# #             )
# #             self.ax.grid(axis='y', linestyle='--', alpha=0.7)
# #             self.ax.legend(fontsize=8)
# #
# #             # Adjust y-axis limits based on the overall min and max values
# #             overall_min = self.summary['min_value'].min()
# #             overall_max = self.summary['max_value'].max()
# #             y_margin = (overall_max - overall_min) * 0.1  # 10% margin
# #             self.ax.set_ylim(overall_min - y_margin, overall_max + y_margin)
# #
# #             # Adjust layout to prevent clipping of tick-labels
# #             self.fig.tight_layout()
# #
# #             # Add interactive tooltips
# #             self.add_tooltips()
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
# #         if self.summary.empty:
# #             return  # No tooltips needed if there's no data
# #
# #         # Tooltip for bars
# #         cursor_bars = mplcursors.cursor(self.bars, hover=True)
# #
# #         @cursor_bars.connect("add")
# #         def on_add_bars(sel):
# #             try:
# #                 # Determine the month from the bar's center x-value
# #                 bar = sel.artist[sel.index]
# #                 month = int(bar.get_x() + bar.get_width() / 2)
# #                 # Filter DataFrame to get the row corresponding to that month
# #                 row = self.summary[self.summary['month'] == month].iloc[0]
# #                 text = (
# #                     f"Giá trị cao nhất: {row['max_value']} {self.unit}\n"
# #                     f"Giá trị thấp nhất: {row['min_value']} {self.unit}\n"
# #                     f"Giá trị trung bình: {row['avg_value']} {self.unit}"
# #                 )
# #                 sel.annotation.set(text=text)
# #                 sel.annotation.get_bbox_patch().set(fc="white")
# #             except Exception as e:
# #                 print(f"Lỗi khi xử lý tooltip cho cột: {e}", file=sys.stderr)
# #
# #         # Tooltip for the average line
# #         cursor_line = mplcursors.cursor(self.line, hover=True)
# #
# #         @cursor_line.connect("add")
# #         def on_add_line(sel):
# #             try:
# #                 # Get x and y values from the selected point
# #                 x, y = sel.target  # sel.target is a tuple (x, y)
# #                 month = int(round(x))
# #                 # Filter DataFrame to get the row corresponding to that month
# #                 row = self.summary[self.summary['month'] == month].iloc[0]
# #                 text = (
# #                     f"Tháng: {row['month']}\n"
# #                     f"Trung bình: {row['avg_value']} {self.unit}\n"
# #                     f"Cao nhất: {row['max_value']} {self.unit}\n"
# #                     f"Thấp nhất: {row['min_value']} {self.unit}"
# #                 )
# #                 sel.annotation.set(text=text)
# #                 sel.annotation.get_bbox_patch().set(fc="white")
# #             except Exception as e:
# #                 print(f"Lỗi khi xử lý tooltip cho đường line: {e}", file=sys.stderr)
# #
# #
# # class MonthChartApp:
# #     def __init__(self, root):
# #         """
# #         Initialize the environment parameters chart application.
# #
# #         :param root: Tkinter root window.
# #         """
# #         self.root = root
# #         self.root.title("Biểu đồ Các Thông Số Môi Trường Theo Năm")
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
# #         # Initialize selected year to the latest available year
# #         self.selected_year = None  # Will be set after fetching available years
# #
# #         # Connect to the SQLite database
# #         self.connect_to_db()
# #
# #         # Fetch available years from the database
# #         self.available_years = self.get_available_years()
# #
# #         if not self.available_years:
# #             print("No data available in the database.")
# #             # Display a message in the GUI and exit
# #             error_frame = ctk.CTkFrame(self.root, width=500, height=100)
# #             error_frame.pack(padx=20, pady=20)
# #             error_label = ctk.CTkLabel(
# #                 error_frame,
# #                 text="No data available in the database.",
# #                 text_color="red",
# #                 font=ctk.CTkFont(size=14, weight="bold")
# #             )
# #             error_label.pack(expand=True)
# #             return
# #
# #         # Set the default selected year to the latest year
# #         self.selected_year = max(self.available_years)
# #
# #         # Create a control frame above the main_frame for year selection
# #         self.control_frame = ctk.CTkFrame(self.root)
# #         self.control_frame.pack(padx=20, pady=10, fill='x')
# #
# #         # Label to display the selected year
# #         self.selected_year_label = ctk.CTkLabel(
# #             self.control_frame,
# #             text=f"Selected Year: {self.selected_year}",
# #             font=ctk.CTkFont(size=14, weight="bold")
# #         )
# #         self.selected_year_label.pack(side='left', padx=10)
# #
# #         # ComboBox to select year
# #         self.year_combobox = ctk.CTkComboBox(
# #             self.control_frame,
# #             values=self.available_years,
# #             command=self.on_year_selected
# #         )
# #         self.year_combobox.set(self.selected_year)  # Set default value
# #         self.year_combobox.pack(side='left', padx=10)
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
# #             print(f"Error connecting to database: {e}", file=sys.stderr)
# #             self.conn = None
# #             self.cursor = None
# #
# #     def get_available_years(self):
# #         """
# #         Fetch distinct years from the database.
# #
# #         :return: List of available years as strings.
# #         """
# #         if not self.conn or not self.cursor:
# #             print("Database connection is not established.", file=sys.stderr)
# #             return []
# #
# #         try:
# #             query = """
# #                 SELECT DISTINCT substr(date_save, 7,4) AS year
# #                 FROM measurements
# #                 WHERE length(substr(date_save, 7,4)) = 4 AND substr(date_save, 7,4) BETWEEN '1900' AND '2100'
# #                 ORDER BY year ASC
# #             """
# #             self.cursor.execute(query)
# #             rows = self.cursor.fetchall()
# #             years = sorted([row[0] for row in rows if row[0].isdigit()])
# #             return years
# #         except sqlite3.Error as e:
# #             print(f"Error fetching available years: {e}", file=sys.stderr)
# #             return []
# #
# #     def on_year_selected(self, selected_year):
# #         """
# #         Callback when a new year is selected from the ComboBox.
# #
# #         :param selected_year: The year selected by the user.
# #         """
# #         self.selected_year = selected_year
# #         # Update the label
# #         self.selected_year_label.configure(text=f"Selected Year: {self.selected_year}")
# #         # Refresh the charts based on the selected year
# #         self.refresh_charts()
# #
# #     def refresh_charts(self):
# #         """
# #         Refresh all charts based on the selected year.
# #         """
# #         # Clear existing charts
# #         for widget in self.main_frame.winfo_children():
# #             widget.destroy()
# #
# #         # Recreate charts with the new year
# #         self.create_charts()
# #
# #     def fetch_data_from_db(self, param_name, param_info):
# #         """
# #         Fetch data from the database for a specific parameter and year.
# #
# #         :param param_name: Name of the parameter.
# #         :param param_info: Information about the parameter (unit, range, color).
# #         :return: DataFrame containing the fetched data.
# #         """
# #         if not self.conn or not self.cursor:
# #             print("Database connection is not established.", file=sys.stderr)
# #             return pd.DataFrame()
# #
# #         try:
# #             # Determine the correct column name based on the parameter
# #             db_column = self.param_to_db_column.get(param_name)
# #             if not db_column:
# #                 print(f"No mapping found for parameter: {param_name}", file=sys.stderr)
# #                 return pd.DataFrame()
# #
# #             # Fetch records for the selected year
# #             # Assuming date_save is in 'dd/mm/yyyy' format
# #             query = f"""
# #                 SELECT date_save, time_save, {db_column}
# #                 FROM measurements
# #                 WHERE substr(date_save, 7,4) = ?
# #             """
# #
# #             self.cursor.execute(query, (self.selected_year,))
# #             rows = self.cursor.fetchall()
# #
# #             # If no data is found, return an empty DataFrame
# #             if not rows:
# #                 print(f"No data found for {param_name} in {self.selected_year}.", file=sys.stderr)
# #                 return pd.DataFrame()
# #
# #             # Create a DataFrame from the fetched data
# #             df = pd.DataFrame(rows, columns=['date_save', 'time_save', db_column])
# #
# #             # Convert the parameter column to numeric, coercing errors
# #             df[db_column] = pd.to_numeric(df[db_column], errors='coerce')
# #
# #             # Drop rows with invalid parameter values
# #             df.dropna(subset=[db_column], inplace=True)
# #
# #             # Combine date and time into a single datetime column
# #             df['timestamp'] = pd.to_datetime(
# #                 df['date_save'] + ' ' + df['time_save'],
# #                 format='%d/%m/%Y %H:%M:%S',
# #                 errors='coerce'  # Handle invalid dates
# #             )
# #
# #             # Drop rows with invalid timestamps
# #             df.dropna(subset=['timestamp'], inplace=True)
# #
# #             # Extract the month from the timestamp
# #             df['month'] = df['timestamp'].dt.month
# #
# #             return df
# #         except sqlite3.Error as e:
# #             print(f"Error fetching data from database: {e}", file=sys.stderr)
# #             return pd.DataFrame()
# #
# #     def process_data(self, df, param_name):
# #         """
# #         Process data to calculate min, max, avg, and corresponding times for each month.
# #
# #         :param df: DataFrame containing data for the parameter.
# #         :param param_name: Name of the parameter.
# #         :return: Summary DataFrame.
# #         """
# #         if df.empty:
# #             # Return an empty summary if there's no data
# #             return pd.DataFrame()
# #
# #         grouped = df.groupby('month')
# #
# #         db_column = self.param_to_db_column.get(param_name)
# #         if not db_column:
# #             print(f"No mapping found for parameter: {param_name}", file=sys.stderr)
# #             return pd.DataFrame()
# #
# #         summary = grouped.agg(
# #             min_value=(db_column, 'min'),
# #             max_value=(db_column, 'max'),
# #             avg_value=(db_column, 'mean')
# #         ).reset_index()
# #
# #         # Find the dates corresponding to min and max values
# #         min_dates = grouped.apply(
# #             lambda x: x.loc[x[db_column].idxmin()]['timestamp'].strftime('%d/%m')
# #         )
# #         max_dates = grouped.apply(
# #             lambda x: x.loc[x[db_column].idxmax()]['timestamp'].strftime('%d/%m')
# #         )
# #
# #         summary['min_date'] = min_dates.values
# #         summary['max_date'] = max_dates.values
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
# #                     text=f"No data available for {param} in {self.selected_year}",
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
# #     app = MonthChartApp(root)
# #
# #     # Run the application
# #     app.run()
# #
# #
# # if __name__ == "__main__":
# #     main()
# # #từ chương trình này, hãy giúp tôi sửa để có thể hiển thị ở trong 1 frame của 1 chương trình khác
# #
# #
#
#
# import customtkinter as ctk
# import pandas as pd
# import matplotlib.pyplot as plt
# from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
# import sqlite3  # Import sqlite3 for database operations
# from datetime import datetime
# import mplcursors
# import sys
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
#         if self.summary.empty:
#             # Display a message if there's no data
#             self.fig, self.ax = plt.subplots(figsize=(6, 4), dpi=100)
#             self.ax.text(0.5, 0.5, 'No data available', horizontalalignment='center',
#                          verticalalignment='center', transform=self.ax.transAxes, fontsize=12, color='red')
#             self.ax.axis('off')
#         else:
#             # Set figsize corresponding to the frame size (width, height) in inches
#             figsize = (6, 4)  # Increased size for better label alignment
#             self.fig, self.ax = plt.subplots(figsize=figsize, dpi=100)
#
#             # Plot the bar chart with limits from min_value to max_value
#             self.bars = self.ax.bar(
#                 self.summary['month'],
#                 self.summary['max_value'] - self.summary['min_value'],
#                 bottom=self.summary['min_value'],
#                 color=self.color,
#                 edgecolor='black',
#                 alpha=0.7,
#                 label='Phạm vi'
#             )
#
#             # Plot the line connecting average values
#             self.line, = self.ax.plot(
#                 self.summary['month'],
#                 self.summary['avg_value'],
#                 color='black',
#                 marker='o',
#                 linestyle='-',
#                 linewidth=2,
#                 label='Trung bình'
#             )
#
#             # Set title and axis labels
#             self.ax.set_title(f'{self.param_name} ({self.unit})', fontsize=12, weight='bold')
#             self.ax.set_xlabel('Tháng', fontsize=10)
#             self.ax.set_ylabel(self.unit, fontsize=10)
#             self.ax.set_xticks(self.summary['month'])
#             self.ax.set_xticklabels(
#                 [f"{month}" for month in self.summary['month']],
#                 rotation=45,
#                 ha='center'  # Changed alignment to 'center' for better alignment with bars
#             )
#             self.ax.grid(axis='y', linestyle='--', alpha=0.7)
#             self.ax.legend(fontsize=8)
#
#             # Adjust y-axis limits based on the overall min and max values
#             overall_min = self.summary['min_value'].min()
#             overall_max = self.summary['max_value'].max()
#             y_margin = (overall_max - overall_min) * 0.1  # 10% margin
#             self.ax.set_ylim(overall_min - y_margin, overall_max + y_margin)
#
#             # Adjust layout to prevent clipping of tick-labels
#             self.fig.tight_layout()
#
#             # Add interactive tooltips
#             self.add_tooltips()
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
#         if self.summary.empty:
#             return  # No tooltips needed if there's no data
#
#         # Tooltip for bars
#         cursor_bars = mplcursors.cursor(self.bars, hover=True)
#
#         @cursor_bars.connect("add")
#         def on_add_bars(sel):
#             try:
#                 # Determine the month from the bar's center x-value
#                 bar = sel.artist[sel.index]
#                 month = int(bar.get_x() + bar.get_width() / 2)
#                 # Filter DataFrame to get the row corresponding to that month
#                 row = self.summary[self.summary['month'] == month].iloc[0]
#                 text = (
#                     f"Giá trị cao nhất: {row['max_value']} {self.unit}\n"
#                     f"Giá trị thấp nhất: {row['min_value']} {self.unit}\n"
#                     f"Giá trị trung bình: {row['avg_value']} {self.unit}"
#                 )
#                 sel.annotation.set(text=text)
#                 sel.annotation.get_bbox_patch().set(fc="white")
#             except Exception as e:
#                 print(f"Lỗi khi xử lý tooltip cho cột: {e}", file=sys.stderr)
#
#         # Tooltip for the average line
#         cursor_line = mplcursors.cursor(self.line, hover=True)
#
#         @cursor_line.connect("add")
#         def on_add_line(sel):
#             try:
#                 # Get x and y values from the selected point
#                 x, y = sel.target  # sel.target is a tuple (x, y)
#                 month = int(round(x))
#                 # Filter DataFrame to get the row corresponding to that month
#                 row = self.summary[self.summary['month'] == month].iloc[0]
#                 text = (
#                     f"Tháng: {row['month']}\n"
#                     f"Trung bình: {row['avg_value']} {self.unit}\n"
#                     f"Cao nhất: {row['max_value']} {self.unit}\n"
#                     f"Thấp nhất: {row['min_value']} {self.unit}"
#                 )
#                 sel.annotation.set(text=text)
#                 sel.annotation.get_bbox_patch().set(fc="white")
#             except Exception as e:
#                 print(f"Lỗi khi xử lý tooltip cho đường line: {e}", file=sys.stderr)
#
#
# class MonthChartApp(ctk.CTkFrame):
#     def __init__(self, parent, *args, **kwargs):
#         """
#         Initialize the environment parameters chart application as a frame.
#
#         :param parent: Parent frame where this chart app will be embedded.
#         """
#         super().__init__(parent, *args, **kwargs)
#         self.pack(fill='both', expand=True)
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
#         # Initialize selected year to the latest available year
#         self.selected_year = None  # Will be set after fetching available years
#
#         # Connect to the SQLite database
#         self.connect_to_db()
#
#         # Fetch available years from the database
#         self.available_years = self.get_available_years()
#
#         if not self.available_years:
#             print("No data available in the database.")
#             # Display a message in the GUI and exit
#             error_frame = ctk.CTkFrame(self, width=500, height=100)
#             error_frame.pack(padx=20, pady=20)
#             error_label = ctk.CTkLabel(
#                 error_frame,
#                 text="No data available in the database.",
#                 text_color="red",
#                 font=ctk.CTkFont(size=14, weight="bold")
#             )
#             error_label.pack(expand=True)
#             return
#
#         # Set the default selected year to the latest year
#         self.selected_year = max(self.available_years)
#
#         # Create a control frame above the main_frame for year selection
#         self.control_frame = ctk.CTkFrame(self)
#         self.control_frame.pack(padx=20, pady=10, fill='x')
#
#         # Label to display the selected year
#         self.selected_year_label = ctk.CTkLabel(
#             self.control_frame,
#             text=f"Selected Year: {self.selected_year}",
#             font=ctk.CTkFont(size=14, weight="bold")
#         )
#         self.selected_year_label.pack(side='left', padx=10)
#
#         # ComboBox to select year
#         self.year_combobox = ctk.CTkComboBox(
#             self.control_frame,
#             values=self.available_years,
#             command=self.on_year_selected
#         )
#         self.year_combobox.set(self.selected_year)  # Set default value
#         self.year_combobox.pack(side='left', padx=10)
#
#         # Create a scrollable frame to contain all the charts
#         self.main_frame = ctk.CTkScrollableFrame(
#             self,
#             width=1050,
#             height=600,
#             border_width=2,
#             border_color="grey",
#             corner_radius=10
#         )
#         self.main_frame.pack(padx=20, pady=20, fill='both', expand=True)
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
#             print(f"Error connecting to database: {e}", file=sys.stderr)
#             self.conn = None
#             self.cursor = None
#
#     def get_available_years(self):
#         """
#         Fetch distinct years from the database.
#
#         :return: List of available years as strings.
#         """
#         if not self.conn or not self.cursor:
#             print("Database connection is not established.", file=sys.stderr)
#             return []
#
#         try:
#             query = """
#                 SELECT DISTINCT substr(date_save, 7,4) AS year
#                 FROM measurements
#                 WHERE length(substr(date_save, 7,4)) = 4 AND substr(date_save, 7,4) BETWEEN '1900' AND '2100'
#                 ORDER BY year ASC
#             """
#             self.cursor.execute(query)
#             rows = self.cursor.fetchall()
#             years = sorted([row[0] for row in rows if row[0].isdigit()])
#             return years
#         except sqlite3.Error as e:
#             print(f"Error fetching available years: {e}", file=sys.stderr)
#             return []
#
#     def on_year_selected(self, selected_year):
#         """
#         Callback when a new year is selected from the ComboBox.
#
#         :param selected_year: The year selected by the user.
#         """
#         self.selected_year = selected_year
#         # Update the label
#         self.selected_year_label.configure(text=f"Selected Year: {self.selected_year}")
#         # Refresh the charts based on the selected year
#         self.refresh_charts()
#
#     def refresh_charts(self):
#         """
#         Refresh all charts based on the selected year.
#         """
#         # Clear existing charts
#         for widget in self.main_frame.winfo_children():
#             widget.destroy()
#
#         # Recreate charts with the new year
#         self.create_charts()
#
#     def fetch_data_from_db(self, param_name, param_info):
#         """
#         Fetch data from the database for a specific parameter and year.
#
#         :param param_name: Name of the parameter.
#         :param param_info: Information about the parameter (unit, range, color).
#         :return: DataFrame containing the fetched data.
#         """
#         if not self.conn or not self.cursor:
#             print("Database connection is not established.", file=sys.stderr)
#             return pd.DataFrame()
#
#         try:
#             # Determine the correct column name based on the parameter
#             db_column = self.param_to_db_column.get(param_name)
#             if not db_column:
#                 print(f"No mapping found for parameter: {param_name}", file=sys.stderr)
#                 return pd.DataFrame()
#
#             # Fetch records for the selected year
#             # Assuming date_save is in 'dd/mm/yyyy' format
#             query = f"""
#                 SELECT date_save, time_save, {db_column}
#                 FROM measurements
#                 WHERE substr(date_save, 7,4) = ?
#             """
#
#             self.cursor.execute(query, (self.selected_year,))
#             rows = self.cursor.fetchall()
#
#             # If no data is found, return an empty DataFrame
#             if not rows:
#                 print(f"No data found for {param_name} in {self.selected_year}.", file=sys.stderr)
#                 return pd.DataFrame()
#
#             # Create a DataFrame from the fetched data
#             df = pd.DataFrame(rows, columns=['date_save', 'time_save', db_column])
#
#             # Convert the parameter column to numeric, coercing errors
#             df[db_column] = pd.to_numeric(df[db_column], errors='coerce')
#
#             # Drop rows with invalid parameter values
#             df.dropna(subset=[db_column], inplace=True)
#
#             # Combine date and time into a single datetime column
#             df['timestamp'] = pd.to_datetime(
#                 df['date_save'] + ' ' + df['time_save'],
#                 format='%d/%m/%Y %H:%M:%S',
#                 errors='coerce'  # Handle invalid dates
#             )
#
#             # Drop rows with invalid timestamps
#             df.dropna(subset=['timestamp'], inplace=True)
#
#             # Extract the month from the timestamp
#             df['month'] = df['timestamp'].dt.month
#
#             return df
#         except sqlite3.Error as e:
#             print(f"Error fetching data from database: {e}", file=sys.stderr)
#             return pd.DataFrame()
#
#     def process_data(self, df, param_name):
#         """
#         Process data to calculate min, max, avg, and corresponding times for each month.
#
#         :param df: DataFrame containing data for the parameter.
#         :param param_name: Name of the parameter.
#         :return: Summary DataFrame.
#         """
#         if df.empty:
#             # Return an empty summary if there's no data
#             return pd.DataFrame()
#
#         grouped = df.groupby('month')
#
#         db_column = self.param_to_db_column.get(param_name)
#         if not db_column:
#             print(f"No mapping found for parameter: {param_name}", file=sys.stderr)
#             return pd.DataFrame()
#
#         summary = grouped.agg(
#             min_value=(db_column, 'min'),
#             max_value=(db_column, 'max'),
#             avg_value=(db_column, 'mean')
#         ).reset_index()
#
#         # Find the dates corresponding to min and max values
#         min_dates = grouped.apply(
#             lambda x: x.loc[x[db_column].idxmin()]['timestamp'].strftime('%d/%m')
#         )
#         max_dates = grouped.apply(
#             lambda x: x.loc[x[db_column].idxmax()]['timestamp'].strftime('%d/%m')
#         )
#
#         summary['min_date'] = min_dates.values
#         summary['max_date'] = max_dates.values
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
#                     text=f"No data available for {param} in {self.selected_year}",
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
#     def close_db_connection(self):
#         """
#         Close the database connection.
#         """
#         if self.conn:
#             self.conn.close()
#             print("Database connection closed.")
#
#
#
# class MainApplication(ctk.CTk):
#     def __init__(self):
#         super().__init__()
#
#         self.title("Main Application with Embedded Charts")
#         self.geometry("1600x1200")  # Adjust size as needed
#
#         # Create a main container frame
#         container = ctk.CTkFrame(self)
#         container.pack(fill='both', expand=True, padx=20, pady=20)
#
#         # Optionally, create other widgets in the main application
#         header = ctk.CTkLabel(container, text="Welcome to the Main Application", font=ctk.CTkFont(size=18, weight="bold"))
#         header.pack(pady=10)
#
#         # Create a sub-frame where the MonthChartApp will be embedded
#         chart_frame = ctk.CTkFrame(container)
#         chart_frame.pack(fill='both', expand=True, pady=20)
#
#         # Initialize and embed the MonthChartApp into chart_frame
#         self.chart_app = MonthChartApp(chart_frame)
#
#         # Optionally, handle window closing to ensure database connections are closed
#         self.protocol("WM_DELETE_WINDOW", self.on_closing)
#
#     def on_closing(self):
#         """
#         Handle the window closing event to ensure all resources are properly released.
#         """
#         # Close the database connection in MonthChartApp
#         self.chart_app.close_db_connection()
#         self.destroy()
#
# if __name__ == "__main__":
#     app = MainApplication()
#     app.mainloop()

##########################################################

import customtkinter as ctk
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import sqlite3  # Import sqlite3 for database operations
from datetime import datetime
import mplcursors
import sys


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
        if self.summary.empty:
            # Display a message if there's no data
            self.fig, self.ax = plt.subplots(figsize=(6, 4), dpi=100)
            self.ax.text(0.5, 0.5, 'No data available', horizontalalignment='center',
                         verticalalignment='center', transform=self.ax.transAxes, fontsize=12, color='red')
            self.ax.axis('off')
        else:
            # Set figsize corresponding to the frame size (width, height) in inches
            figsize = (6, 4)  # Increased size for better label alignment
            self.fig, self.ax = plt.subplots(figsize=figsize, dpi=100)

            # Plot the bar chart with limits from min_value to max_value
            self.bars = self.ax.bar(
                self.summary['month'],
                self.summary['max_value'] - self.summary['min_value'],
                bottom=self.summary['min_value'],
                color=self.color,
                edgecolor='black',
                alpha=0.7,
                label='Phạm vi'
            )

            # Plot the line connecting average values
            self.line, = self.ax.plot(
                self.summary['month'],
                self.summary['avg_value'],
                color='black',
                marker='o',
                linestyle='-',
                linewidth=2,
                label='Trung bình'
            )

            # Set title and axis labels
            self.ax.set_title(f'{self.param_name} ({self.unit})', fontsize=12, weight='bold')
            self.ax.set_xlabel('Tháng', fontsize=10)
            self.ax.set_ylabel(self.unit, fontsize=10)
            self.ax.set_xticks(self.summary['month'])
            self.ax.set_xticklabels(
                [f"{month}" for month in self.summary['month']],
                rotation=45,
                ha='center'  # Changed alignment to 'center' for better alignment with bars
            )
            self.ax.grid(axis='y', linestyle='--', alpha=0.7)
            self.ax.legend(fontsize=8)

            # Adjust y-axis limits based on the overall min and max values
            overall_min = self.summary['min_value'].min()
            overall_max = self.summary['max_value'].max()
            y_margin = (overall_max - overall_min) * 0.1  # 10% margin
            self.ax.set_ylim(overall_min - y_margin, overall_max + y_margin)

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
        if self.summary.empty:
            return  # No tooltips needed if there's no data

        # Tooltip for bars
        cursor_bars = mplcursors.cursor(self.bars, hover=True)

        @cursor_bars.connect("add")
        def on_add_bars(sel):
            try:
                # Determine the month from the bar's center x-value
                bar = sel.artist[sel.index]
                month = int(bar.get_x() + bar.get_width() / 2)
                # Filter DataFrame to get the row corresponding to that month
                row = self.summary[self.summary['month'] == month].iloc[0]
                text = (
                    f"Giá trị cao nhất: {row['max_value']} {self.unit}\n"
                    f"Giá trị thấp nhất: {row['min_value']} {self.unit}\n"
                    f"Giá trị trung bình: {row['avg_value']} {self.unit}"
                )
                sel.annotation.set(text=text)
                sel.annotation.get_bbox_patch().set(fc="white")
            except Exception as e:
                print(f"Lỗi khi xử lý tooltip cho cột: {e}", file=sys.stderr)

        # Tooltip for the average line
        # cursor_line = mplcursors.cursor(self.line, hover=True)
        #
        # @cursor_line.connect("add")
        # def on_add_line(sel):
        #     try:
        #         # Get x and y values from the selected point
        #         x, y = sel.target  # sel.target is a tuple (x, y)
        #         month = int(round(x))
        #         # Filter DataFrame to get the row corresponding to that month
        #         row = self.summary[self.summary['month'] == month].iloc[0]
        #         text = (
        #             f"Tháng: {row['month']}\n"
        #             f"Trung bình: {row['avg_value']} {self.unit}\n"
        #             f"Cao nhất: {row['max_value']} {self.unit}\n"
        #             f"Thấp nhất: {row['min_value']} {self.unit}"
        #         )
        #         sel.annotation.set(text=text)
        #         sel.annotation.get_bbox_patch().set(fc="white")
        #     except Exception as e:
        #         print(f"Lỗi khi xử lý tooltip cho đường line: {e}", file=sys.stderr)


class MonthChartApp(ctk.CTkFrame):
    def __init__(self, parent, database_name='database1.db', *args, **kwargs):
        """
        Initialize the environment parameters chart application as a frame.

        :param parent: Parent frame where this chart app will be embedded.
        :param database_name: Name of the SQLite database file to connect to.
        """
        super().__init__(parent, *args, **kwargs)
        self.pack(fill='both', expand=True)

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

        # Store the current database name
        self.current_database = database_name

        # Connect to the SQLite database
        self.connect_to_db()

        # Fetch available years from the database
        self.available_years = self.get_available_years()

        # Create a control frame above the main_frame for year selection
        self.control_frame = ctk.CTkFrame(self)
        self.control_frame.pack(padx=20, pady=10, fill='x')

        # Label to display the selected year
        self.selected_year_label = ctk.CTkLabel(
            self.control_frame,
            text="Selected Year: N/A",
            font=ctk.CTkFont(size=14, weight="bold")
        )
        self.selected_year_label.pack(side='left', padx=10)

        # ComboBox to select year
        self.year_combobox = ctk.CTkComboBox(
            self.control_frame,
            values=self.available_years,
            command=self.on_year_selected
        )
        if self.available_years:
            self.selected_year = max(self.available_years)
            self.selected_year_label.configure(text=f"Selected Year: {self.selected_year}")
            self.year_combobox.set(self.selected_year)  # Set default value
        else:
            self.selected_year = None
            self.year_combobox.set('')
        self.year_combobox.pack(side='left', padx=10)

        # Create a scrollable frame to contain all the charts
        self.main_frame = ctk.CTkScrollableFrame(
            self,
            width=1400,
            height=800,
            border_width=2,
            border_color="grey",
            corner_radius=10
        )
        self.main_frame.pack(padx=20, pady=20, fill='both', expand=True)

        # Configure grid layout within the scrollable frame
        for i in range(3):
            self.main_frame.grid_rowconfigure(i, weight=1, pad=10)
        for j in range(2):
            self.main_frame.grid_columnconfigure(j, weight=1, pad=10)

        if self.available_years:
            # Create and embed all charts
            self.create_charts()
        else:
            # Display a message in the GUI
            error_frame = ctk.CTkFrame(self.main_frame, width=500, height=100)
            error_frame.grid(row=0, column=0, padx=10, pady=10, sticky="nsew", columnspan=2)
            error_label = ctk.CTkLabel(
                error_frame,
                text=f"No data available in the database {self.current_database}.",
                wraplength=480,
                text_color="red",
                font=ctk.CTkFont(size=12, weight="bold")
            )
            error_label.pack(expand=True)

    def connect_to_db(self, database_name=None):
        """
        Establish a connection to the SQLite database.

        :param database_name: Name of the database file. If None, use the current database.
        """
        if database_name:
            self.current_database = database_name

        # Close existing connection if any
        if hasattr(self, 'conn') and self.conn:
            self.conn.close()
            print(f"Closed connection to {self.current_database}.")

        try:
            self.conn = sqlite3.connect(self.current_database)
            self.cursor = self.conn.cursor()
            print(f"Connected to {self.current_database} successfully.")
        except sqlite3.Error as e:
            print(f"Error connecting to database {self.current_database}: {e}", file=sys.stderr)
            self.conn = None
            self.cursor = None

    def get_available_years(self):
        """
        Fetch distinct years from the database.

        :return: List of available years as strings.
        """
        if not self.conn or not self.cursor:
            print("Database connection is not established.", file=sys.stderr)
            return []

        try:
            query = """
                SELECT DISTINCT substr(date_save, 7,4) AS year
                FROM measurements
                WHERE length(substr(date_save, 7,4)) = 4 AND substr(date_save, 7,4) BETWEEN '1900' AND '2100'
                ORDER BY year ASC
            """
            self.cursor.execute(query)
            rows = self.cursor.fetchall()
            years = sorted([row[0] for row in rows if row[0].isdigit()])
            return years
        except sqlite3.Error as e:
            print(f"Error fetching available years: {e}", file=sys.stderr)
            return []

    def on_year_selected(self, selected_year):
        """
        Callback when a new year is selected from the ComboBox.

        :param selected_year: The year selected by the user.
        """
        self.selected_year = selected_year
        # Update the label
        self.selected_year_label.configure(text=f"Selected Year: {self.selected_year}")
        # Refresh the charts based on the selected year
        self.refresh_charts()

    def refresh_charts(self):
        """
        Refresh all charts based on the selected year.
        """
        # Clear existing charts
        for widget in self.main_frame.winfo_children():
            widget.destroy()

        if self.available_years:
            # Recreate charts with the new year
            self.create_charts()
        else:
            # Display a message in the GUI
            error_frame = ctk.CTkFrame(self.main_frame, width=500, height=100)
            error_frame.grid(row=0, column=0, padx=10, pady=10, sticky="nsew", columnspan=2)
            error_label = ctk.CTkLabel(
                error_frame,
                text=f"No data available in the database {self.current_database}.",
                wraplength=480,
                text_color="red",
                font=ctk.CTkFont(size=12, weight="bold")
            )
            error_label.pack(expand=True)

    def fetch_data_from_db(self, param_name, param_info):
        """
        Fetch data from the database for a specific parameter and year.

        :param param_name: Name of the parameter.
        :param param_info: Information about the parameter (unit, range, color).
        :return: DataFrame containing the fetched data.
        """
        if not self.conn or not self.cursor:
            print("Database connection is not established.", file=sys.stderr)
            return pd.DataFrame()

        try:
            # Determine the correct column name based on the parameter
            db_column = self.param_to_db_column.get(param_name)
            if not db_column:
                print(f"No mapping found for parameter: {param_name}", file=sys.stderr)
                return pd.DataFrame()

            # Fetch records for the selected year
            # Assuming date_save is in 'dd/mm/yyyy' format
            query = f"""
                SELECT date_save, time_save, {db_column}
                FROM measurements
                WHERE substr(date_save, 7,4) = ?
            """

            self.cursor.execute(query, (self.selected_year,))
            rows = self.cursor.fetchall()

            # If no data is found, return an empty DataFrame
            if not rows:
                print(f"No data found for {param_name} in {self.selected_year}.", file=sys.stderr)
                return pd.DataFrame()

            # Create a DataFrame from the fetched data
            df = pd.DataFrame(rows, columns=['date_save', 'time_save', db_column])

            # Convert the parameter column to numeric, coercing errors
            df[db_column] = pd.to_numeric(df[db_column], errors='coerce')

            # Drop rows with invalid parameter values
            df.dropna(subset=[db_column], inplace=True)

            # Combine date and time into a single datetime column
            df['timestamp'] = pd.to_datetime(
                df['date_save'] + ' ' + df['time_save'],
                format='%d/%m/%Y %H:%M:%S',
                errors='coerce'  # Handle invalid dates
            )

            # Drop rows with invalid timestamps
            df.dropna(subset=['timestamp'], inplace=True)

            # Extract the month from the timestamp
            df['month'] = df['timestamp'].dt.month

            return df
        except sqlite3.Error as e:
            print(f"Error fetching data from database: {e}", file=sys.stderr)
            return pd.DataFrame()

    def process_data(self, df, param_name):
        """
        Process data to calculate min, max, avg, and corresponding times for each month.

        :param df: DataFrame containing data for the parameter.
        :param param_name: Name of the parameter.
        :return: Summary DataFrame.
        """
        if df.empty:
            # Return an empty summary if there's no data
            return pd.DataFrame()

        grouped = df.groupby('month')

        db_column = self.param_to_db_column.get(param_name)
        if not db_column:
            print(f"No mapping found for parameter: {param_name}", file=sys.stderr)
            return pd.DataFrame()

        summary = grouped.agg(
            min_value=(db_column, 'min'),
            max_value=(db_column, 'max'),
            avg_value=(db_column, 'mean')
        ).reset_index()

        # Find the dates corresponding to min and max values
        min_dates = grouped.apply(
            lambda x: x.loc[x[db_column].idxmin()]['timestamp'].strftime('%d/%m')
        )
        max_dates = grouped.apply(
            lambda x: x.loc[x[db_column].idxmax()]['timestamp'].strftime('%d/%m')
        )

        summary['min_date'] = min_dates.values
        summary['max_date'] = max_dates.values
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
                    text=f"No data available for {param} in {self.selected_year}",
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

    def switch_database(self, new_database):
        """
        Switch to a new database and refresh the charts.

        :param new_database: Name of the new database file.
        """
        if new_database == self.current_database:
            print(f"Already connected to {new_database}. No action taken.")
            return

        print(f"Switching database to {new_database}...")
        self.connect_to_db(new_database)

        # Fetch available years from the new database
        self.available_years = self.get_available_years()

        if not self.available_years:
            print(f"No data available in the database {new_database}.")
            # Display a message in the GUI and clear existing charts
            for widget in self.main_frame.winfo_children():
                widget.destroy()

            error_frame = ctk.CTkFrame(self.main_frame, width=500, height=100)
            error_frame.grid(row=0, column=0, padx=10, pady=10, sticky="nsew", columnspan=2)
            error_label = ctk.CTkLabel(
                error_frame,
                text=f"No data available in the database {new_database}.",
                text_color="red",
                font=ctk.CTkFont(size=14, weight="bold")
            )
            error_label.pack(expand=True)
            # Update the year selection ComboBox to be empty
            self.selected_year = None
            self.selected_year_label.configure(text="Selected Year: N/A")
            self.year_combobox.configure(values=[])
            self.year_combobox.set('')
            return

        # Update the year selection ComboBox with new years
        self.selected_year = max(self.available_years)
        self.selected_year_label.configure(text=f"Selected Year: {self.selected_year}")
        self.year_combobox.configure(values=self.available_years)
        self.year_combobox.set(self.selected_year)

        # Refresh the charts with the new database and selected year
        self.refresh_charts()

    def close_db_connection(self):
        """
        Close the database connection.
        """
        if hasattr(self, 'conn') and self.conn:
            self.conn.close()
            print(f"Database connection to {self.current_database} closed.")


class MainApplication(ctk.CTk):
    def __init__(self):
        super().__init__()

        self.title("Main Application with Embedded Charts")
        self.geometry("1600x1200")  # Adjust size as needed

        # Create a main container frame
        container = ctk.CTkFrame(self)
        container.pack(fill='both', expand=True, padx=20, pady=20)

        # Create a header frame for the welcome message and Radiobuttons
        header_frame = ctk.CTkFrame(container)
        header_frame.pack(fill='x', pady=10)

        # Welcome label
        header = ctk.CTkLabel(
            header_frame,
            text="Welcome to the Main Application",
            font=ctk.CTkFont(size=18, weight="bold")
        )
        header.pack(side='left', padx=10)

        # Database selection Radiobuttons
        db_selection_frame = ctk.CTkFrame(header_frame)
        db_selection_frame.pack(side='right', padx=10)

        db_label = ctk.CTkLabel(
            db_selection_frame,
            text="Select Database:",
            font=ctk.CTkFont(size=12, weight="bold")
        )
        db_label.pack(side='left', padx=5)

        # Variable to hold the selected database
        self.selected_db = ctk.StringVar(value='database1.db')  # Default selection

        # Define available databases
        self.databases = ['database1.db', 'database2.db', 'database3.db']

        # Create Radiobuttons for each database
        for db in self.databases:
            rb = ctk.CTkRadioButton(
                db_selection_frame,
                text=db,
                variable=self.selected_db,
                value=db,
                command=self.on_database_selected
            )
            rb.pack(side='left', padx=5)

        # Create a sub-frame where the MonthChartApp will be embedded
        chart_frame = ctk.CTkFrame(container)
        chart_frame.pack(fill='both', expand=True, pady=20)

        # Initialize and embed the MonthChartApp into chart_frame
        # Start with the default selected database
        self.chart_app = MonthChartApp(chart_frame, database_name=self.selected_db.get())

        # Optionally, handle window closing to ensure database connections are closed
        self.protocol("WM_DELETE_WINDOW", self.on_closing)

    def on_database_selected(self):
        """
        Callback when a new database is selected via Radiobutton.
        """
        new_db = self.selected_db.get()
        print(f"Radiobutton selected: {new_db}")
        self.chart_app.switch_database(new_db)

    def on_closing(self):
        """
        Handle the window closing event to ensure all resources are properly released.
        """
        # Close the database connection in MonthChartApp
        self.chart_app.close_db_connection()
        self.destroy()


if __name__ == "__main__":
    app = MainApplication()
    app.mainloop()

