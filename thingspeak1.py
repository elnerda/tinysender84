import serial
import time
import sys
import glob
import platform
import httplib,urllib
x=''
y=''
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
		time.sleep(2)
		print"Connected"
		print"Press Ctrl+C to close"
		while 1:
			
			print "Getting Data"
			ser.write("t")
			x=ser.read(10)
			print x
			time.sleep(1)
			ser.write("p")
			y=ser.read(10)
			print y
			time.sleep(1)
			print "Posting..."
			time.sleep(1)
			params = urllib.urlencode ({'field1': x,'field2' : y, 'key':'K877A6ECOC6ZDBW5'})
			headers = {"Content-typZZe": "appliction/x-www-form-urlencoded","Accept": "text/plain"}
			conn=httplib.HTTPConnection("api.thingspeak.com:80")
			try:
				conn.request("POST", "/update", params, headers)
				response = conn.getresponse()
				print x,y
				print response.status, response.reason
				print"Updating channel complete!"
				conn.close()
					
			except:
				print "connection failed"
					
			time.sleep(30)
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
		
		
		
		
		
		
		
		
		
	
		
		
		
		
		
