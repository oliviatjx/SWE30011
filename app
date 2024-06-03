from flask import Flask, render_template, request, jsonify
import mysql.connector as MySQLdb
import statistics
import serial

app = Flask(__name__)

# Configure the database connection
Conn = MySQLdb.connect(host="localhost", user="jiaxuan", password="", database="iot_db")
cursor = Conn.cursor()

# Initialize the serial connection with Arduino
ser = serial.Serial('/dev/ttyS0', 9600)

# Create the table if it doesn't exist
cursor.execute("""
CREATE TABLE IF NOT EXISTS SensorData (
    id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    distance FLOAT NOT NULL,
    ir_value FLOAT NOT NULL
)
""")

@app.route('/')
def index():
    query = "SELECT * FROM SensorData ORDER BY timestamp DESC"
    cursor.execute(query)
    results = cursor.fetchall()
    return render_template('index.html', data=results)

@app.route('/data')
def data():
    query = "SELECT id, timestamp, distance, ir_value FROM SensorData ORDER BY timestamp DESC"
    cursor.execute(query)
    rows = cursor.fetchall()
    data = []
    for row in rows:
        data.append({
            "id": row[0],
            "timestamp": row[1].isoformat(),  
            "distance": row[2],
            "ir_value": row[3]
        })
    return jsonify(data)

# Statistics added here
@app.route('/mean_ir_value', methods=['POST'])
def mean_ir_value():
    query = "SELECT ir_value FROM SensorData"
    cursor.execute(query)
    ir_values = [row[0] for row in cursor.fetchall()]
    mean_value = statistics.mean(ir_values) if ir_values else 0
    return jsonify({'mean_ir_value': mean_value})

# Arduino Interaction
@app.route('/off_led', methods=['POST'])
def off_ledpin():
    # Send the command to turn off the LED
    ser.write(b'L')
    return jsonify({'status': 'LED off command sent'})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
