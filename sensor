import serial
import mysql.connector
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

# Database connection
dbConn = mysql.connector.connect(
    host="localhost",
    user="jiaxuan",
    password="",
    database="iot_db"
) or die("Could not connect to database")

cursor = dbConn.cursor()
ser = serial.Serial('/dev/ttyS0', 9600)

# Email setup
smtp_server = "smtp.gmail.com"  
smtp_port = 587
email_user = "olivewillbehacked@gmail.com"
email_password = "dpjg lfkb ojzs wdoz"
email_recipient = "olivia.oliviatay21@gmail.com"

def send_email(distance, ir_value):
    msg = MIMEMultipart()
    msg['From'] = email_user
    msg['To'] = email_recipient
    msg['Subject'] = "Pest Alert in Your Garden"
    body = f"Pest detected! Distance: {distance}, IR Value: {ir_value}"
    msg.attach(MIMEText(body, 'plain'))
    text = msg.as_string()

    try:
        server = smtplib.SMTP(smtp_server, smtp_port)
        server.starttls()
        server.login(email_user, email_password)
        server.sendmail(email_user, email_recipient, text)
        server.quit()
        print("Email sent successfully")
    except Exception as e:
        print(f"Failed to send email: {e}")

def off_led(ir_value):
    ser.write(b"LED_OFF\n")

while True:
    try:
        data = ser.readline().decode('utf-8').strip()
        print(f"Received from Arduino: {data}")
        values = data.split(', ')
        if len(values) == 2:
            distance, irSensorValue = values
            
            # Insert the data into the database
            sql = "INSERT INTO SensorData (distance, ir_value) VALUES (%s, %s)"
            cursor.execute(sql, (distance, irSensorValue))
            dbConn.commit()
            print(f"Saved to database: {distance}, {irSensorValue}")

            # Define conditions for triggering an alert
            if int(distance) < 10.0 and int(irSensorValue) > 100:  
                send_email(distance, irSensorValue)
            
            # Conditional statement added
            if int(irSensorValue) > 250:
                off_led(irSensorValue)
        else:
            print(f"Unexpected data format: {data}")
    except Exception as e:
        print(f"Error: {e}")

# Close the cursor and connection when done
cursor.close()
dbConn.close()
