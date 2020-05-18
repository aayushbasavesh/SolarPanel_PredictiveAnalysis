#!/usr/bin/python
##############
## Script listens to serial port and writes contents into a file
##############
## requires pySerial to be installed 
import serial
import time
import sys
import csv


timestamp = time.strftime("%Y-%m-%d-%H-%M-%S")
ser = '/dev/ttyACM0'
baud_rate = 9600; #In arduino, Serial.begin(baud_rate)
filepath = "/home/pi/Documents/final_project/source_data/V1.2/" + timestamp + ".csv";
input_stream = serial.Serial(ser, baud_rate,)
fields = ['Date&Time','Voltage','SC_Current']
print("Writing output to file: " + filepath + "\n")
csv.register_dialect('mydialect', delimiter = ',', skipinitialspace = True, lineterminator = '\r\n', quoting = csv.QUOTE_ALL)
with open(filepath, 'a+') as f:
            writer = csv.writer(f, dialect =  'mydialect')
            writer.writerow(fields)
f.close()


while 1:
    line = input_stream.readline();
    data_list = line.split(",")
    timestamp = time.strftime("%Y/%m/%d-%H:%M:%S")
    str(timestamp)
    data_list.insert(0,timestamp)
    print(data_list);
    with open(filepath, 'a+') as f:
        for item in data_list:
		str(item)
		f.write(item)
	 	f.write(",")
	f.write('\n')
	#writer = csv.writer(f, dialect = 'mydialect')
        f.close()
