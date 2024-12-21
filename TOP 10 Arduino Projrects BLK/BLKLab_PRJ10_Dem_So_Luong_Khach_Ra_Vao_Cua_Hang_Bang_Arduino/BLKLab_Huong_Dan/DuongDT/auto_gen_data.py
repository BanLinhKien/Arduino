import sqlite3
from datetime import datetime
import paho.mqtt.client as mqtt

# Hàm tạo cơ sở dữ liệu và bảng measurements
def create_database(tendatabase):
    conn = sqlite3.connect(tendatabase)
    cursor = conn.cursor()

    # Tạo bảng measurements nếu chưa tồn tại
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS measurements (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            date_save TEXT,
            time_save TEXT,
            temp REAL,
            humi REAL,
            co REAL,
            pm25 REAL,
            nh3 REAL,
            airpresser REAL
        )
    ''')

    conn.commit()
    conn.close()

# Hàm chèn dữ liệu vào cơ sở dữ liệu
def insert_data(tendatabase, data):
    conn = sqlite3.connect(tendatabase)
    cursor = conn.cursor()

    # Chèn dữ liệu vào bảng
    cursor.execute('''
        INSERT INTO measurements (date_save, time_save, temp, humi, co, pm25, nh3, airpresser)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?)
    ''', (data["date_save"], data["time_save"], data["temp"], data["humi"], data["co"], data["pm25"], data["nh3"], data["airpresser"]))

    conn.commit()
    conn.close()

# Hàm xử lý khi nhận được tin nhắn từ topic
def on_message(client, userdata, msg):
    raw_data = msg.payload.decode('utf-8')
    print(f"Received raw data: {raw_data}")

    # Tách dữ liệu
    values = raw_data.split('-')

    # Kiểm tra định dạng dữ liệu
    if len(values) == 7:
        # Lấy thời gian thực
        now = datetime.now()
        date_save = now.strftime("%d/%m/%Y")  # Ngày lưu (DD-MM-YYYY)
        time_save = now.strftime("%H:%M:%S")  # Thời gian lưu (HH:MM:SS)

        # Chuẩn bị dữ liệu
        node = values[0]
        data = {
            "date_save": date_save,
            "time_save": time_save,
            "pm25": float(values[1]),
            "co": float(values[2]),
            "nh3": float(values[3]),
            "airpresser": float(values[4]),
            "humi": float(values[5]),
            "temp": float(values[6]),
        }

        # Xử lý theo Node
        if node == "Node_1":
            insert_data("database1.db", data)
            print(f"Data: {data}")
            print("Dữ liệu đã được lưu vào database1.")
        elif node == "Node_2":
            insert_data("database2.db", data)
            print(f"Data: {data}")
            print("Dữ liệu đã được lưu vào database2.")
        elif node == "Node_3":
            insert_data("database3.db", data)
            print(f"Data: {data}")
            print("Dữ liệu đã được lưu vào database3.")
        else:
            print("Node không được hỗ trợ!")
    else:
        print("Error: Data format is incorrect!")

# Cấu hình MQTT
BROKER = "broker.hivemq.com"
PORT = 1883
TOPIC = "ESP_SIM4G/DATA"

# Hàm xử lý khi kết nối thành công
def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT Broker!" if rc == 0 else f"Failed to connect, return code {rc}")
    # Đăng ký nhận dữ liệu từ topic
    client.subscribe(TOPIC)
    print(f"Subscribed to topic: {TOPIC}")

# Khởi tạo MQTT Client
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

# Tạo cơ sở dữ liệu
create_database("database1.db")
create_database("database2.db")
create_database("database3.db")

# Kết nối đến MQTT broker
client.connect(BROKER, PORT, 60)

# Chạy vòng lặp lắng nghe MQTT
try:
    print("Listening for MQTT messages...")
    client.loop_forever()
except KeyboardInterrupt:
    print("Disconnected from broker.")
    client.disconnect()
