from bottle import Bottle, html_escape, run,template
import web
import time
import getlightvalue

import Adafruit_DHT

dhtpin = 23 #BCM is 23, wiring is 4, BORAD is 16
lightpin = 18 #BCM is 18, wiring is 1, BORAD is 12
app = Bottle()

@app.route('/')
def hello():
    humidity, temperature = Adafruit_DHT.read_retry(11, dhtpin)
    lightFlag = getlightvalue.read_pin(lightpin)
    localtime = time.localtime(time.time())
    time_now = '{}:{}:{}'.format(localtime.tm_hour, localtime.tm_min, localtime.tm_sec)
    #str_1 = html_str.html.format(time_now,wendu,shidu)
    return template(web.html, time=time_now,temperature=temperature,humidity=humidity,lightFlag=lightFlag)

run(app, host='192.168.1.103', port=8080) 
