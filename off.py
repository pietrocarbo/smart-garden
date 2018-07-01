import os
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

os.system("pkill -f on.py")

GPIO.setmode(GPIO.BCM)

GPIO.setwarnings(False)

GPIO.setup(channel, GPIO.IN)

print('Stopping valve..')

writeActive(False)
