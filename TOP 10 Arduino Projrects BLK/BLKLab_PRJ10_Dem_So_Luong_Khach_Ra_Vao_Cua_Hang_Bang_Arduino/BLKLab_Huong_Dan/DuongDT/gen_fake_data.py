# # import sqlite3
# # from datetime import datetime, timedelta
# # import random
# #
# # # Tạo kết nối đến database
# # conn = sqlite3.connect('database.db')
# # cursor = conn.cursor()
# #
# # # Tạo bảng với các trường yêu cầu
# # cursor.execute('''
# # CREATE TABLE IF NOT EXISTS data (
# #     thoi_gian TEXT,
# #     gio TEXT,
# #     nhietdo REAL,
# #     doam REAL,
# #     co REAL,
# #     pm25 REAL
# # )
# # ''')
# #
# # # Thiết lập thời gian bắt đầu (1/1/2023)
# # start_date = datetime(2023, 1, 1, 0, 0, 0)
# # end_date = datetime.now()
# #
# # # Lặp qua khoảng thời gian từ 1/1/2023 đến ngày hiện tại, cách mỗi dữ liệu 20s
# # current_time = start_date
# #
# # while current_time <= end_date:
# #     thoi_gian = current_time.strftime("%d/%m/%Y")  # Định dạng ngày (dd/mm/yyyy)
# #     gio = current_time.strftime("%H:%M:%S")  # Định dạng giờ (hh:mm:ss)
# #
# #     # Tạo dữ liệu ngẫu nhiên cho các trường còn lại
# #     nhietdo = round(random.uniform(20.0, 35.0), 2)  # Nhiệt độ (20 đến 35 độ C)
# #     doam = round(random.uniform(40.0, 90.0), 2)  # Độ ẩm (40% đến 90%)
# #     co = round(random.uniform(0.0, 1.5), 2)  # CO (0 đến 1.5 ppm)
# #     pm25 = round(random.uniform(0.0, 100.0), 2)  # PM2.5 (0 đến 100 µg/m³)
# #
# #     # Thêm dữ liệu vào bảng
# #     cursor.execute('''
# #     INSERT INTO data (thoi_gian, gio, nhietdo, doam, co, pm25)
# #     VALUES (?, ?, ?, ?, ?, ?)
# #     ''', (thoi_gian, gio, nhietdo, doam, co, pm25))
# #
# #     # Cập nhật thời gian (cộng thêm 20s)
# #     current_time += timedelta(seconds=20)
# #
# # # Commit và đóng kết nối
# # conn.commit()
# # conn.close()
# #
# # # Kết quả: Dữ liệu đã được tạo và chèn vào trong bảng 'data' của 'database.db'.
#
#
# import sqlite3
# import random
# from datetime import datetime, timedelta
# import time
#
# def create_database(db_name):
#     conn = sqlite3.connect(db_name)
#     c = conn.cursor()
#     # Tối ưu hóa SQLite để tăng tốc độ chèn dữ liệu
#     c.execute('PRAGMA synchronous = OFF')
#     c.execute('PRAGMA journal_mode = MEMORY')
#     # Tạo bảng nếu chưa tồn tại
#     c.execute('''
#         CREATE TABLE IF NOT EXISTS measurements (
#             date_save TEXT,
#             time_save  TEXT,
#             temp REAL,
#             humi REAL,
#             co REAL,
#             pm25 REAL,
#             nh3 REAL,
#             airpresser REAL
#         )
#     ''')
#     conn.commit()
#     return conn, c
#
# def generate_random_data(start_datetime, end_datetime, batch_size=100000):
#     data_list = []
#     current_datetime = start_datetime
#     total_records = 0
#
#     while current_datetime <= end_datetime:
#         date_str = current_datetime.strftime('%d/%m/%Y')
#         time_str = current_datetime.strftime('%H:%M:%S')
#         # Sinh dữ liệu ngẫu nhiên
#         temp = round(random.uniform(-10.0, 40.0), 2)            # Nhiệt độ từ -10 đến 40 độ C
#         humi = round(random.uniform(0.0, 100.0), 2)          # Độ ẩm từ 0% đến 100%
#         co = round(random.uniform(0.0, 50.0), 2)             # CO từ 0 đến 50 ppm
#         pm25 = round(random.uniform(0.0, 500.0), 2)          # PM2.5 từ 0 đến 500 µg/m³
#         nh3 = round(random.uniform(0.0, 100.0), 2)           # NH3 từ 0 đến 100 ppm
#         airpresser = round(random.uniform(950.0, 1050.0), 2) # Áp suất không khí từ 950 đến 1050 hPa
#         data_list.append((date_str, time_str, temp, humi, co, pm25, nh3, airpresser))
#         total_records += 1
#
#         # Nếu đạt đến kích thước lô, trả về và bắt đầu một lô mới
#         if len(data_list) >= batch_size:
#             yield data_list
#             data_list = []
#
#         # Tăng thời gian lên 20 giây
#         current_datetime += timedelta(seconds=20)
#
#     # Trả về phần còn lại
#     if data_list:
#         yield data_list
#
# def insert_data(cursor, data):
#     cursor.executemany('''
#         INSERT INTO measurements (date_save, time_save, temp, humi, co, pm25, nh3, airpresser)
#         VALUES (?, ?, ?, ?, ?, ?, ?, ?)
#     ''', data)
#
# def main():
#     db_name = 'database3.db'
#     conn, cursor = create_database(db_name)
#
#     # Định nghĩa ngày bắt đầu và ngày kết thúc
#     start_datetime = datetime.strptime('01/01/2023 00:00:00', '%d/%m/%Y %H:%M:%S')
#     end_datetime = datetime.strptime('15/12/2025 23:59:59', '%d/%m/%Y %H:%M:%S')  # Ngày hiện tại là 15/12/2024
#
#     print(f"Bắt đầu tạo dữ liệu từ {start_datetime.strftime('%d/%m/%Y %H:%M:%S')} đến {end_datetime.strftime('%d/%m/%Y %H:%M:%S')}")
#
#     total_inserted = 0
#     start_time = time.time()
#
#     try:
#         for batch in generate_random_data(start_datetime, end_datetime):
#             insert_data(cursor, batch)
#             conn.commit()
#             total_inserted += len(batch)
#             elapsed = time.time() - start_time
#             print(f"Đã chèn {total_inserted} bản ghi. Thời gian đã trôi: {elapsed/60:.2f} phút.")
#     except KeyboardInterrupt:
#         print("Quá trình tạo dữ liệu đã bị dừng bởi người dùng.")
#     finally:
#         conn.close()
#         print("Đã hoàn tất chèn dữ liệu và đóng kết nối cơ sở dữ liệu.")
#
# if __name__ == "__main__":
#     main()


import sqlite3
import random
from datetime import datetime, timedelta
import time


def create_database(db_name):
    """
    Creates a SQLite database with the specified name and initializes the measurements table.

    Parameters:
        db_name (str): The name of the database file to create or connect to.

    Returns:
        tuple: A tuple containing the connection and cursor objects.
    """
    conn = sqlite3.connect(db_name)
    c = conn.cursor()
    # Optimize SQLite for faster data insertion
    c.execute('PRAGMA synchronous = OFF')
    c.execute('PRAGMA journal_mode = MEMORY')
    # Create table if it doesn't exist
    c.execute('''
        CREATE TABLE IF NOT EXISTS measurements (
            date_save TEXT,
            time_save  TEXT,
            temp REAL,
            humi REAL,
            co REAL,
            pm25 REAL,
            nh3 REAL,
            airpresser REAL
        )
    ''')
    conn.commit()
    return conn, c


def generate_random_data(start_datetime, end_datetime, batch_size=100000):
    """
    Generates random measurement data between start_datetime and end_datetime.

    Parameters:
        start_datetime (datetime): The starting datetime for data generation.
        end_datetime (datetime): The ending datetime for data generation.
        batch_size (int): Number of records per batch.

    Yields:
        list: A list of tuples containing measurement data.
    """
    data_list = []
    current_datetime = start_datetime
    total_records = 0

    while current_datetime <= end_datetime:
        date_str = current_datetime.strftime('%d/%m/%Y')
        time_str = current_datetime.strftime('%H:%M:%S')
        # Generate random data
        temp = round(random.uniform(-10.0, 40.0), 2)  # Temperature from -10 to 40°C
        humi = round(random.uniform(0.0, 100.0), 2)  # Humidity from 0% to 100%
        co = round(random.uniform(0.0, 50.0), 2)  # CO from 0 to 50 ppm
        pm25 = round(random.uniform(0.0, 500.0), 2)  # PM2.5 from 0 to 500 µg/m³
        nh3 = round(random.uniform(0.0, 100.0), 2)  # NH3 from 0 to 100 ppm
        airpresser = round(random.uniform(950.0, 1050.0), 2)  # Air pressure from 950 to 1050 hPa
        data_list.append((date_str, time_str, temp, humi, co, pm25, nh3, airpresser))
        total_records += 1

        # Yield the batch if the batch size is reached
        if len(data_list) >= batch_size:
            yield data_list
            data_list = []

        # Increment time by 20 seconds
        current_datetime += timedelta(seconds=20)

    # Yield any remaining data
    if data_list:
        yield data_list


def insert_data(cursor, data):
    """
    Inserts a batch of records into the measurements table.

    Parameters:
        cursor (sqlite3.Cursor): The database cursor.
        data (list): A list of tuples containing measurement data.
    """
    cursor.executemany('''
        INSERT INTO measurements (date_save, time_save, temp, humi, co, pm25, nh3, airpresser)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?)
    ''', data)


def insert_single_data(cursor, date_save, time_save, temp, humi, co, pm25, nh3, airpresser):
    """
    Inserts a single record into the measurements table.

    Parameters:
        cursor (sqlite3.Cursor): The database cursor.
        date_save (str): The date string in 'dd/mm/yyyy' format.
        time_save (str): The time string in 'HH:MM:SS' format.
        temp (float): Temperature in degrees Celsius.
        humi (float): Humidity percentage.
        co (float): CO concentration in ppm.
        pm25 (float): PM2.5 concentration in µg/m³.
        nh3 (float): NH3 concentration in ppm.
        airpresser (float): Air pressure in hPa.
    """
    cursor.execute('''
        INSERT INTO measurements (date_save, time_save, temp, humi, co, pm25, nh3, airpresser)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?)
    ''', (date_save, time_save, temp, humi, co, pm25, nh3, airpresser))


def main():
    """
    Main function to create and populate multiple SQLite databases with random measurement data.
    """
    # Define the list of database names you want to create and populate
    db_names = ['database1.db', 'database2.db', 'database3.db']

    # Define start and end dates
    start_datetime = datetime.strptime('01/10/2024 00:00:00', '%d/%m/%Y %H:%M:%S')
    end_datetime = datetime.strptime('16/12/2024 23:59:59', '%d/%m/%Y %H:%M:%S')  # Current date is 15/12/2024

    print(
        f"Starting data creation from {start_datetime.strftime('%d/%m/%Y %H:%M:%S')} to {end_datetime.strftime('%d/%m/%Y %H:%M:%S')}")

    # Iterate over each database name
    for db_name in db_names:
        print(f"\nProcessing database: {db_name}")
        conn, cursor = create_database(db_name)
        total_inserted = 0
        start_time = time.time()

        try:
            for batch in generate_random_data(start_datetime, end_datetime):
                insert_data(cursor, batch)
                conn.commit()
                total_inserted += len(batch)
                elapsed = time.time() - start_time
                print(f"Inserted {total_inserted} records into {db_name}. Elapsed time: {elapsed / 60:.2f} minutes.")

            # Example of inserting a single record into the current database
            # single_record = {
            #     'date_save': '16/12/2024',
            #     'time_save': '12:36:56',
            #     'temp': 40,
            #     'humi': 70,
            #     'co': 5.5,
            #     'pm25': 35.2,
            #     'nh3': 10.1,
            #     'airpresser': 1013.25
            # }
            #
            # insert_single_data(
            #     cursor,
            #     single_record['date_save'],
            #     single_record['time_save'],
            #     single_record['temp'],
            #     single_record['humi'],
            #     single_record['co'],
            #     single_record['pm25'],
            #     single_record['nh3'],
            #     single_record['airpresser']
            # )
            conn.commit()
            total_inserted += 1
            print(f"Inserted a single record into {db_name}. Total records inserted: {total_inserted}")

        except KeyboardInterrupt:
            print(f"Data creation process for {db_name} was interrupted by the user.")
        except Exception as e:
            print(f"An error occurred while processing {db_name}: {e}")
        finally:
            conn.close()
            print(f"Data insertion completed and database {db_name} connection closed.")

    print("\nAll database processing completed.")


if __name__ == "__main__":
    main()

