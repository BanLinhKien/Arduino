import customtkinter as ctk

class LoginForm(ctk.CTk):
    def __init__(self, on_success):
        super().__init__()

        self.title("Form Đăng Nhập")
        self.geometry("400x250")
        self.on_success = on_success  # Hàm được gọi khi đăng nhập thành công

        # Cấu hình giao diện
        self.grid_columnconfigure(1, weight=1)

        # Tiêu đề
        self.label_title = ctk.CTkLabel(self, text="Đăng Nhập Hệ Thống", font=("Arial", 20, "bold"))
        self.label_title.grid(row=0, column=0, columnspan=2, pady=20)

        # Tên đăng nhập
        self.label_username = ctk.CTkLabel(self, text="Tên đăng nhập:")
        self.label_username.grid(row=1, column=0, padx=10, pady=10, sticky="e")
        self.entry_username = ctk.CTkEntry(self, placeholder_text="Nhập tên đăng nhập", width=200)
        self.entry_username.grid(row=1, column=1, padx=10, pady=10, sticky="w")

        # Mật khẩu
        self.label_password = ctk.CTkLabel(self, text="Mật khẩu:")
        self.label_password.grid(row=2, column=0, padx=10, pady=10, sticky="e")
        self.entry_password = ctk.CTkEntry(self, placeholder_text="Nhập mật khẩu", show="*", width=200)
        self.entry_password.grid(row=2, column=1, padx=10, pady=10, sticky="w")

        # Nút đăng nhập
        self.button_login = ctk.CTkButton(self, text="Đăng Nhập", command=self.login)
        self.button_login.grid(row=3, column=0, columnspan=2, pady=20)

        # Thông báo
        self.label_message = ctk.CTkLabel(self, text="", text_color="red")
        self.label_message.grid(row=4, column=0, columnspan=2)

    def login(self):
        username = self.entry_username.get()
        password = self.entry_password.get()

        if username == "" and password == "":
            self.label_message.configure(text="Đăng nhập thành công!", text_color="green")
            self.destroy()  # Đóng cửa sổ đăng nhập
            self.on_success()  # Gọi hàm trong chương trình chính
        else:
            self.label_message.configure(text="Sai tên đăng nhập hoặc mật khẩu!", text_color="red")
