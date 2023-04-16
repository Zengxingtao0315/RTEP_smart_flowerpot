from bottle import Bottle, html_escape, run,template
import web
import time
import getlightvalue

import Adafruit_DHT

dhtpin = 4 #BCM is 4, wiring is 7, BORAD is 7
lightpin = 17 #BCM is 17, wiring is 0, BORAD is 11
app = Bottle()

@app.route('/')
def hello():
    humidity, temperature = Adafruit_DHT.read_retry(11, pin)
    lightFlag = getlightvalue.read_pin(lightpin)
    localtime = time.localtime(time.time())
    time_now = '{}:{}:{}'.format(localtime.tm_hour, localtime.tm_min, localtime.tm_sec)
    #str_1 = html_str.html.format(time_now,wendu,shidu)
    return template(web.html, time=time_now,temperature=temperature,humidity=humidity,lightFlag=lightFlag)

run(app, host='192.168.1.103', port=8080) 