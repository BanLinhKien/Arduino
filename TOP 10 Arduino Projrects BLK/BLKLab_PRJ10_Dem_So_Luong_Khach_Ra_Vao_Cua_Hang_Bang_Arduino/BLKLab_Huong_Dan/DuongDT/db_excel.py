# import customtkinter as ctk
# from tkinter import ttk, messagebox, filedialog
# import sqlite3
# from datetime import datetime
# import pandas as pd
#
# # Thiết lập giao diện
# ctk.set_appearance_mode("System")  # Có thể chọn "System", "Dark", "Light"
# ctk.set_default_color_theme("blue")  # Có thể chọn "blue", "green", "dark-blue"
#
#
# class DatabaseExcel(ctk.CTkFrame):
#     def __init__(self, parent):
#         super().__init__(parent)
#         # Kết nối tới cơ sở dữ liệu
#         self.conn = sqlite3.connect('database.db')
#         self.cursor = self.conn.cursor()
#
#         # Khung nhập thời gian bắt đầu
#         frame_start = ctk.CTkFrame(self)
#         frame_start.pack(pady=10, padx=10, fill="x")
#
#         lbl_start = ctk.CTkLabel(frame_start, text="Từ (dd/mm/yyyy hh:mm:ss):")
#         lbl_start.pack(side="left", padx=5)
#
#         self.entry_start_date = ctk.CTkEntry(frame_start, width=150)
#         self.entry_start_date.pack(side="left", padx=5)
#         self.entry_start_time = ctk.CTkEntry(frame_start, width=100)
#         self.entry_start_time.pack(side="left", padx=5)
#
#         # Khung nhập thời gian kết thúc
#         frame_end = ctk.CTkFrame(self)
#         frame_end.pack(pady=10, padx=10, fill="x")
#
#         lbl_end = ctk.CTkLabel(frame_end, text="Đến (dd/mm/yyyy hh:mm:ss):")
#         lbl_end.pack(side="left", padx=5)
#
#         self.entry_end_date = ctk.CTkEntry(frame_end, width=150)
#         self.entry_end_date.pack(side="left", padx=5)
#         self.entry_end_time = ctk.CTkEntry(frame_end, width=100)
#         self.entry_end_time.pack(side="left", padx=5)
#
#         # Khung chứa nút lọc và xuất Excel
#         frame_buttons = ctk.CTkFrame(self)
#         frame_buttons.pack(pady=10, padx=10, fill="x")
#
#         # Nút lọc dữ liệu
#         btn_filter = ctk.CTkButton(frame_buttons, text="Lọc Dữ Liệu", command=self.filter_data)
#         btn_filter.pack(side="left", padx=5)
#
#         # Nút xuất Excel
#         btn_export = ctk.CTkButton(frame_buttons, text="Xuất Excel", command=self.export_to_excel)
#         btn_export.pack(side="left", padx=5)
#
#         # Khung chứa Treeview và Scrollbar
#         frame_tree = ctk.CTkFrame(self)
#         frame_tree.pack(pady=20, padx=20, fill="both", expand=True)
#
#         # Tạo Style để điều chỉnh rowheight (tuỳ chọn)
#         style = ttk.Style()
#         style.configure("Treeview", rowheight=25)  # Điều chỉnh chiều cao hàng lên 25 pixel
#
#         # Tạo Treeview với height=20 (hiển thị 20 dòng)
#         self.tree = ttk.Treeview(
#             frame_tree,
#             columns=("date_save", "time_save", "temp", "humi", "co", "pm25", "nh3", "airpresser"),
#             show='headings',
#             height=15  # Điều chỉnh số lượng dòng hiển thị
#         )
#         self.tree.heading("date_save", text="Ngày")
#         self.tree.heading("time_save", text="Giờ")
#         self.tree.heading("temp", text="Nhiệt Độ")
#         self.tree.heading("humi", text="Độ Ẩm")
#         self.tree.heading("co", text="CO")
#         self.tree.heading("pm25", text="PM2.5")
#         self.tree.heading("nh3", text="NH3")
#         self.tree.heading("airpresser", text="Áp Suất Không Khí")
#
#         self.tree.column("date_save", width=130, anchor="center")
#         self.tree.column("time_save", width=130, anchor="center")
#         self.tree.column("temp", width=130, anchor="center")
#         self.tree.column("humi", width=130, anchor="center")
#         self.tree.column("co", width=130, anchor="center")
#         self.tree.column("pm25", width=130, anchor="center")
#         self.tree.column("nh3", width=130, anchor="center")
#         self.tree.column("airpresser", width=140, anchor="center")
#
#         # Tạo Scrollbar dọc
#         scrollbar = ttk.Scrollbar(frame_tree, orient="vertical", command=self.tree.yview)
#         self.tree.configure(yscroll=scrollbar.set)
#
#         # Sắp xếp Treeview và Scrollbar trong khung
#         self.tree.pack(side="left", fill="both", expand=True)
#         scrollbar.pack(side="right", fill="y")
#
#         # Thêm thông báo khi bảng trống (tùy chọn)
#         self.lbl_no_data = ctk.CTkLabel(
#             frame_tree,
#             text="Chưa có dữ liệu. Vui lòng lọc dữ liệu để hiển thị.",
#             text_color="gray"
#         )
#         self.lbl_no_data.pack(pady=20)
#
#         # Không tải dữ liệu ban đầu để bảng trống
#         # self.load_data()  # Đã được loại bỏ
#
#     def load_data(self, records=None):
#         # Xóa dữ liệu cũ
#         for row in self.tree.get_children():
#             self.tree.delete(row)
#
#         if records is None:
#             return  # Không tải dữ liệu nếu không có bản ghi
#
#         for row in records:
#             self.tree.insert("", "end", values=row)
#
#         # Ẩn thông báo khi có dữ liệu
#         if records:
#             self.lbl_no_data.pack_forget()
#         else:
#             self.lbl_no_data.pack(pady=20)
#
#     def filter_data(self):
#         start_date = self.entry_start_date.get()
#         start_time = self.entry_start_time.get()
#         end_date = self.entry_end_date.get()
#         end_time = self.entry_end_time.get()
#
#         try:
#             # Chuyển đổi chuỗi nhập vào thành đối tượng datetime
#             start_dt = datetime.strptime(f"{start_date} {start_time}", "%d/%m/%Y %H:%M:%S")
#             end_dt = datetime.strptime(f"{end_date} {end_time}", "%d/%m/%Y %H:%M:%S")
#
#             if start_dt > end_dt:
#                 messagebox.showerror("Lỗi", "Thời gian bắt đầu phải nhỏ hơn thời gian kết thúc.")
#                 return
#
#             # Truy vấn dữ liệu từ cơ sở dữ liệu
#             self.cursor.execute("SELECT * FROM measurements")
#             all_records = self.cursor.fetchall()
#
#             # Lọc dữ liệu theo khoảng thời gian
#             filtered = []
#             for record in all_records:
#                 record_date = record[0]  # date_save
#                 record_time = record[1]  # time_save
#                 record_dt = datetime.strptime(f"{record_date} {record_time}", "%d/%m/%Y %H:%M:%S")
#                 if start_dt <= record_dt <= end_dt:
#                     filtered.append(record)
#
#             if not filtered:
#                 messagebox.showinfo("Thông Báo", "Không tìm thấy dữ liệu trong khoảng thời gian đã chọn.")
#
#             self.load_data(filtered)
#
#         except ValueError:
#             messagebox.showerror("Lỗi", "Vui lòng nhập đúng định dạng ngày và giờ (dd/mm/yyyy hh:mm:ss).")
#
#     def export_to_excel(self):
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
#         df = pd.DataFrame(records,
#                           columns=["Ngày", "Giờ", "Nhiệt Độ", "Độ Ẩm", "CO", "PM2.5", "NH3", "Áp Suất Không Khí"])
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
#         self.conn.close()
#         self.destroy()
#
#
# # ctk.set_appearance_mode("System")  # Có thể chọn "System", "Dark", "Light"
# # ctk.set_default_color_theme("blue")  # Có thể chọn "blue", "green", "dark-blue"
# #
# #
# # class MainApp(ctk.CTk):
# #     def __init__(self):
# #         super().__init__()
# #         self.title("Chương Trình Chính")
# #         self.geometry("1200x800")  # Kích thước tùy chỉnh cho chương trình chính
# #
# #         # Tạo một Frame để chứa DataDisplayFrame
# #         main_frame = ctk.CTkFrame(self)
# #         main_frame.pack(pady=20, padx=20, fill="both", expand=True)
# #
# #         # Tạo và nhúng DataDisplayFrame vào main_frame
# #         self.data_display = DataDisplayFrame(main_frame)
# #         self.data_display.pack(fill="both", expand=True)
# #
# #         # Thêm các widget khác của chương trình chính nếu cần
# #         # Ví dụ: Một menu, sidebar, etc.
# #
# #     def on_closing(self):
# #         # Đóng kết nối cơ sở dữ liệu nếu cần
# #         try:
# #             self.data_display.conn.close()
# #         except:
# #             pass
# #         self.destroy()
# #
# #
# # if __name__ == "__main__":
# #     app = MainApp()
# #     app.protocol("WM_DELETE_WINDOW", app.on_closing)
# #     app.mainloop()


import customtkinter as ctk
from tkinter import ttk, messagebox, filedialog
import sqlite3
from datetime import datetime
import pandas as pd

# Thiết lập giao diện
ctk.set_appearance_mode("System")  # Có thể chọn "System", "Dark", "Light"
ctk.set_default_color_theme("blue")  # Có thể chọn "blue", "green", "dark-blue"


class DatabaseExcel(ctk.CTkFrame):
    def __init__(self, parent, db_path='database1.db'):
        super().__init__(parent)
        self.parent = parent
        self.db_path = db_path
        self.conn = None
        self.cursor = None
        self.connect_database(self.db_path)

        # Khung nhập thời gian bắt đầu
        frame_start = ctk.CTkFrame(self)
        frame_start.pack(pady=10, padx=10, fill="x")

        lbl_start = ctk.CTkLabel(frame_start, text="Từ (dd/mm/yyyy hh:mm:ss):")
        lbl_start.pack(side="left", padx=5)

        self.entry_start_date = ctk.CTkEntry(frame_start, width=150)
        self.entry_start_date.pack(side="left", padx=5)
        self.entry_start_time = ctk.CTkEntry(frame_start, width=100)
        self.entry_start_time.pack(side="left", padx=5)

        # Khung nhập thời gian kết thúc
        frame_end = ctk.CTkFrame(self)
        frame_end.pack(pady=10, padx=10, fill="x")

        lbl_end = ctk.CTkLabel(frame_end, text="Đến (dd/mm/yyyy hh:mm:ss):")
        lbl_end.pack(side="left", padx=5)

        self.entry_end_date = ctk.CTkEntry(frame_end, width=150)
        self.entry_end_date.pack(side="left", padx=5)
        self.entry_end_time = ctk.CTkEntry(frame_end, width=100)
        self.entry_end_time.pack(side="left", padx=5)

        # Khung chứa nút lọc và xuất Excel
        frame_buttons = ctk.CTkFrame(self)
        frame_buttons.pack(pady=10, padx=10, fill="x")

        # Nút lọc dữ liệu
        btn_filter = ctk.CTkButton(frame_buttons, text="Lọc Dữ Liệu", command=self.filter_data)
        btn_filter.pack(side="left", padx=5)

        # Nút xuất Excel
        btn_export = ctk.CTkButton(frame_buttons, text="Xuất Excel", command=self.export_to_excel)
        btn_export.pack(side="left", padx=5)

        # Khung chứa Treeview và Scrollbar
        frame_tree = ctk.CTkFrame(self)
        frame_tree.pack(pady=20, padx=20, fill="both", expand=True)

        # Tạo Style để điều chỉnh rowheight (tuỳ chọn)
        style = ttk.Style()
        style.configure("Treeview", rowheight=25)  # Điều chỉnh chiều cao hàng lên 25 pixel

        # Tạo Treeview với height=20 (hiển thị 20 dòng)
        self.tree = ttk.Treeview(
            frame_tree,
            columns=("date_save", "time_save", "temp", "humi", "co", "pm25", "nh3", "airpresser"),
            show='headings',
            height=15  # Điều chỉnh số lượng dòng hiển thị
        )
        self.tree.heading("date_save", text="Ngày")
        self.tree.heading("time_save", text="Giờ")
        self.tree.heading("temp", text="Nhiệt Độ")
        self.tree.heading("humi", text="Độ Ẩm")
        self.tree.heading("co", text="CO")
        self.tree.heading("pm25", text="PM2.5")
        self.tree.heading("nh3", text="NH3")
        self.tree.heading("airpresser", text="Áp Suất Không Khí")

        self.tree.column("date_save", width=130, anchor="center")
        self.tree.column("time_save", width=130, anchor="center")
        self.tree.column("temp", width=130, anchor="center")
        self.tree.column("humi", width=120, anchor="center")
        self.tree.column("co", width=120, anchor="center")
        self.tree.column("pm25", width=120, anchor="center")
        self.tree.column("nh3", width=120, anchor="center")
        self.tree.column("airpresser", width=130, anchor="center")

        # Tạo Scrollbar dọc
        scrollbar = ttk.Scrollbar(frame_tree, orient="vertical", command=self.tree.yview)
        self.tree.configure(yscroll=scrollbar.set)

        # Sắp xếp Treeview và Scrollbar trong khung
        self.tree.pack(side="left", fill="both", expand=True)
        scrollbar.pack(side="right", fill="y")

        # Thêm thông báo khi bảng trống (tùy chọn)
        self.lbl_no_data = ctk.CTkLabel(
            frame_tree,
            text="Chưa có dữ liệu. Vui lòng lọc dữ liệu để hiển thị.",
            text_color="gray"
        )
        self.lbl_no_data.pack(pady=20)

        # Không tải dữ liệu ban đầu để bảng trống
        # self.load_data()  # Đã được loại bỏ

    def connect_database(self, db_path):
        """Connect to the specified SQLite database."""
        try:
            if self.conn:
                self.conn.close()
            self.conn = sqlite3.connect(db_path)
            self.cursor = self.conn.cursor()
        except sqlite3.Error as e:
            messagebox.showerror("Lỗi Kết Nối CSDL", f"Không thể kết nối tới cơ sở dữ liệu:\n{e}")
            self.conn = None
            self.cursor = None

    def load_data(self, records=None):
        """Load data into the Treeview."""
        # Xóa dữ liệu cũ
        for row in self.tree.get_children():
            self.tree.delete(row)

        if records is None:
            return  # Không tải dữ liệu nếu không có bản ghi

        for row in records:
            self.tree.insert("", "end", values=row)

        # Ẩn thông báo khi có dữ liệu
        if records:
            self.lbl_no_data.pack_forget()
        else:
            self.lbl_no_data.pack(pady=20)

    def filter_data(self):
        """Filter data based on the specified date and time range."""
        if not self.cursor:
            messagebox.showerror("Lỗi", "Không có kết nối tới cơ sở dữ liệu.")
            return

        start_date = self.entry_start_date.get()
        start_time = self.entry_start_time.get()
        end_date = self.entry_end_date.get()
        end_time = self.entry_end_time.get()

        try:
            # Chuyển đổi chuỗi nhập vào thành đối tượng datetime
            start_dt = datetime.strptime(f"{start_date} {start_time}", "%d/%m/%Y %H:%M:%S")
            end_dt = datetime.strptime(f"{end_date} {end_time}", "%d/%m/%Y %H:%M:%S")

            if start_dt > end_dt:
                messagebox.showerror("Lỗi", "Thời gian bắt đầu phải nhỏ hơn thời gian kết thúc.")
                return

            # Truy vấn dữ liệu từ cơ sở dữ liệu
            self.cursor.execute("SELECT * FROM measurements")
            all_records = self.cursor.fetchall()

            # Lọc dữ liệu theo khoảng thời gian
            filtered = []
            for record in all_records:
                record_date = record[0]  # date_save
                record_time = record[1]  # time_save
                try:
                    record_dt = datetime.strptime(f"{record_date} {record_time}", "%d/%m/%Y %H:%M:%S")
                    if start_dt <= record_dt <= end_dt:
                        filtered.append(record)
                except ValueError:
                    continue  # Bỏ qua các bản ghi có định dạng ngày giờ không hợp lệ

            if not filtered:
                messagebox.showinfo("Thông Báo", "Không tìm thấy dữ liệu trong khoảng thời gian đã chọn.")

            self.load_data(filtered)

        except ValueError:
            messagebox.showerror("Lỗi", "Vui lòng nhập đúng định dạng ngày và giờ (dd/mm/yyyy hh:mm:ss).")

    def export_to_excel(self):
        """Export the current data in the Treeview to an Excel file."""
        # Lấy tất cả các bản ghi hiện tại trong Treeview
        records = []
        for row_id in self.tree.get_children():
            row = self.tree.item(row_id)['values']
            records.append(row)

        if not records:
            messagebox.showwarning("Cảnh Báo", "Không có dữ liệu để xuất.")
            return

        # Tạo DataFrame từ dữ liệu
        df = pd.DataFrame(records,
                          columns=["Ngày", "Giờ", "Nhiệt Độ", "Độ Ẩm", "CO", "PM2.5", "NH3", "Áp Suất Không Khí"])

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

    def set_database(self, db_path):
        """Set a new database and reload data."""
        self.db_path = db_path
        self.connect_database(self.db_path)
        self.load_data()  # Clear current data

    def on_closing(self):
        """Handle the closing of the frame."""
        if self.conn:
            self.conn.close()
        self.destroy()


class MainApp(ctk.CTk):
    def __init__(self):
        super().__init__()
        self.title("Chương Trình Chính")
        self.geometry("1200x800")  # Kích thước tùy chỉnh cho chương trình chính

        # Tạo một Frame để chứa các widget chính
        main_frame = ctk.CTkFrame(self)
        main_frame.pack(pady=20, padx=20, fill="both", expand=True)

        # Tạo và nhúng DatabaseExcel vào main_frame
        self.data_display = DatabaseExcel(main_frame, db_path='database1.db')  # Khởi tạo với database1.db
        self.data_display.pack(fill="both", expand=True)

        # Thêm các widget khác của chương trình chính nếu cần
        # Ví dụ: Một menu, sidebar, etc.

        # Tạo một Frame cho Radiobuttons
        frame_radiobuttons = ctk.CTkFrame(self)
        frame_radiobuttons.pack(pady=10, padx=10, fill="x")

        # Biến để lưu trạng thái lựa chọn của Radiobuttons
        self.selected_db = ctk.StringVar(value='database1.db')

        # Định nghĩa các lựa chọn database
        databases = [
            ("Cơ Sở Dữ Liệu 1", "database1.db"),
            ("Cơ Sở Dữ Liệu 2", "database2.db"),
            ("Cơ Sở Dữ Liệu 3", "database3.db")
        ]

        # Tạo Radiobuttons
        for text, db in databases:
            rb = ctk.CTkRadioButton(
                frame_radiobuttons,
                text=text,
                variable=self.selected_db,
                value=db,
                command=self.change_database
            )
            rb.pack(side="left", padx=10)

    def change_database(self):
        """Handle the change of database when a Radiobutton is selected."""
        selected_db = self.selected_db.get()
        self.data_display.set_database(selected_db)
        messagebox.showinfo("Cập Nhật Cơ Sở Dữ Liệu", f"Đã chuyển sang {selected_db}.")

    def on_closing(self):
        """Handle the closing of the application."""
        self.data_display.on_closing()
        self.destroy()


if __name__ == "__main__":
    app = MainApp()
    app.protocol("WM_DELETE_WINDOW", app.on_closing)
    app.mainloop()
