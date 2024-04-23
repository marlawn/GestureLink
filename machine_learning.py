# Author: Marlon Vergara

import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVC

import serial

data = pd.read_csv('train.csv')
X = data.iloc[:, :-1].values
y = data.iloc[:, -1].values
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)
svm_model = SVC(kernel='linear', random_state=42)
svm_model.fit(X_train_scaled, y_train)

gesture = ''
ser_sensor = serial.Serial('COM3', 115200)
columns = ['pinky', 'ring', 'middle', 'index', 'thumb']
for _ in range(10):
    ser_sensor.readline()
while True:
    data = ser_sensor.readline().decode('utf-8').strip().split(',')
    if len(data) != 5:
        continue
    data_dict = dict(zip(columns, data))
    df = pd.DataFrame([data_dict])
    x = df.values
    x_scaled = scaler.transform(x)
    gesture = svm_model.predict(x_scaled)
    gesture_str = str(gesture[0])

    ser_sensor.write(gesture_str.encode('utf-8'))
