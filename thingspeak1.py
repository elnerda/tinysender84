import serial
import time
import sys
import glob
import platform
def serial_ports():
    
    if sys.platform.startswith('win'):
        ports = ['COM' + str(i + 1) for i in range(256)]

    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this is to exclude your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')

    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result
	

result = serial_ports()
platform =platform.system()
print platform



if not result:
	print"No Serialports available"
	print"Press Ctrl+C to close"
else:
	if platform.startswith('Win'):
		print"Available Serialports"
		print result
		var=input("Please select your Serialport:COM")
		var2='COM'+str(var)
		print"Connecting to "+var2
		ser= serial.Serial(var2,9600,timeout=1)
		time.sleep(3)
		print"Connected"
		print"Press Ctrl+C to close"
		while 1:
		
			
			ser.write("t")
			t=ser.read(10);
			time.sleep(0.5)
			ser.write("p")
			p=ser.read(10)
			time.sleep(0.5)
			print t
			print p
		
		
		
		
		
		
		
	if platform.startswith('Lin'):
		print"Are you on a Raspberry Pi B+?"
		var=raw_input("y/n ")
		if var.startswith('y'):
			a=serial_ports()
			print"Automatically connecting to Hardware Serialport "+a[1]
			ser = serial.Serial(a[1],9600,timeout=1)
			time.sleep(3)
			print"Connected"
			print"Press Ctrl+C to close"
		else:
			print"Available Serialports"
			print result
			print"Press Ctrl+C to close"
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		