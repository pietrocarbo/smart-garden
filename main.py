import json
import time
import requests
import datetime
from pprint import pprint

def loadSettings():
    with open('settings.json') as f:
        data = json.load(f)
    return data

while True:
    settings = loadSettings()

    if not settings['active']:
        if settings['week']['days'][datetime.datetime.today().isocalendar()[2] - 1]:
            now = datetime.datetime.now()
            scheduled = datetime.datetime.strptime(settings['week']['time'], '%H:%M').time()
            if (now.hour + 2) == scheduled.hour and now.minute == scheduled.minute:
                print('It\'s time to annaffiare')
                response = requests.get('http://localhost:1880/on?minutes=' + str(settings['week']['duration']))

    time.sleep(45)
    


