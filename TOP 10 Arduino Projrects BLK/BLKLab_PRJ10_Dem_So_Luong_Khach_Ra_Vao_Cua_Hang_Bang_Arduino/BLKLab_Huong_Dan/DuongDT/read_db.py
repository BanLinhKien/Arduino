import sqlite3

def read_all_data(db_path, table_name):
    try:
        # Kết nối đến cơ sở dữ liệu SQLite
        conn = sqlite3.connect(db_path)
        cursor = conn.cursor()

        # Truy vấn tất cả dữ liệu từ bảng
        query = f"SELECT * FROM {table_name}"
        cursor.execute(query)

        # Lấy tên các cột
        column_names = [description[0] for description in cursor.description]

        # Lấy tất cả các hàng dữ liệu
        rows = cursor.fetchall()

        # Hiển thị dữ liệu
        print(f"Dữ liệu từ bảng '{table_name}':")
        print(column_names)
        for row in rows:
            print(row)

    except sqlite3.Error as e:
        print(f"Lỗi khi truy cập cơ sở dữ liệu: {e}")

    finally:
        if conn:
            conn.close()

if __name__ == "__main__":
    db_path = 'database1.db'      # Đường dẫn đến file database.db
    table_name = 'measurements'  # Thay thế bằng tên bảng của bạn
    read_all_data(db_path, table_name)
