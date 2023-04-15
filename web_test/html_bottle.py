from bottle import Bottle, html_escape, run,template
import web
import dht11_drive,time


app = Bottle()

@app.route('/wendu')
def hello():
    shidu, wendu = dht11_drive.dht11()
    localtime = time.localtime(time.time())
    time_now = '{}:{}:{}'.format(localtime.tm_hour, localtime.tm_min, localtime.tm_sec)
    #str_1 = html_str.html.format(time_now,wendu,shidu)
    return template(web.html, time=time_now, wendu=wendu,shidu=shidu)

run(app, host='127.0.0.1', port=8080) 