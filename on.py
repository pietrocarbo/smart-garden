import time
import sys
import RPi.GPIO as GPIO
import json

channel = 4

def writeActive(active):
    with open('/home/pi/settings.json', 'r+') as f:
        data = json.load(f)
        data[u'active'] = active
        f.seek(0)
        json.dump(data, f)
        f.truncate()
       

if len(sys.argv) == 2 and sys.argv[1].isdigit():
    minutes = int(sys.argv[1])
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(channel, GPIO.OUT)
    GPIO.output(channel, GPIO.HIGH)
    print('Started for ' + str(minutes) + ' minutes..')
    
    writeActive(True)

    while(minutes > 0):
        time.sleep(60)
        minutes -= 1

    print('Stopping valve..')
    GPIO.cleanup(channel)

    writeActive(False)
else:
    print('No minutes specified')
    exit()
