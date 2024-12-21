import sqlite3

# Kết nối đến cơ sở dữ liệu
conn = sqlite3.connect('database1.db')

# Tạo con trỏ để thực thi các lệnh SQL
cursor = conn.cursor()

# Truy vấn để lấy tất cả các bảng trong cơ sở dữ liệu
cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
tables = cursor.fetchall()

print("Danh sách dữ liệu trong các bảng:")
for table in tables:
    table_name = table[0]
    print(f"\nBảng: {table_name}")

    # Truy vấn tất cả dữ liệu trong bảng hiện tại
    cursor.execute(f"SELECT * FROM {table_name};")
    rows = cursor.fetchall()

    # In ra dữ liệu
    for row in rows:
        print(row)

# Đóng kết nối
conn.close()
