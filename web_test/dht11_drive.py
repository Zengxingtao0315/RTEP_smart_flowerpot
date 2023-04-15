import RPi.GPIO as GPIO
import time

#############################################################
dhtpin = 4 # 这里填写GPIO号（BCM引脚编号模式）    手动改变此行
#############################################################

GPIO.setmode(GPIO.BCM)  # 设置使用BCM引脚编号模式

def main(dhtpin):
    dht_data = [0] * 5
    cnt = 7
    idx = 0
    f = 0.0
    if GPIO.getmode() is None:
        GPIO.setmode(GPIO.BCM)

    for i in range(5):
        dht_data[i] = 0

    # pull pin down for 20 milliseconds
    GPIO.setup(dhtpin, GPIO.OUT)
    GPIO.output(dhtpin, GPIO.LOW)
    time.sleep(0.018)

    # then pull it up for 40 microseconds
    GPIO.output(dhtpin, GPIO.HIGH)
    time.sleep(0.00004)

    # prepare to read the pin
    GPIO.setup(dhtpin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

    # ACKNOWLEDGE or TIMEOUT
    loopCnt = 10000
    while GPIO.input(dhtpin) == GPIO.LOW:
        if loopCnt == 0:
            return -2
        loopCnt -= 1

    loopCnt = 10000
    while GPIO.input(dhtpin) == GPIO.HIGH:
        if loopCnt == 0:
            return -2
        loopCnt -= 1

    for i in range(40):
        loopCnt = 10000
        while GPIO.input(dhtpin) == GPIO.LOW:
            if loopCnt == 0:
                return -2
            loopCnt -= 1

        t = time.time()

        loopCnt = 10000
        while GPIO.input(dhtpin) == GPIO.HIGH:
            if loopCnt == 0:
                return -2
            loopCnt -= 1

        if (time.time() - t) > 0.00004:
            dht_data[idx] |= (1 << cnt)
        if cnt == 0:   # next byte?
            cnt = 7    # restart at MSB
            idx += 1      # next byte!
        else:
            cnt -= 1

    if dht_data[4] == ((dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) & 0xFF):
        f = dht_data[2] * 9. / 5. + 32
        humidity = dht_data[0] + dht_data[1] * 0.1
        temperature = dht_data[2] + dht_data[3] * 0.1
        print("Humidity = {:.1f} %, Temperature = {:.1f} C ({:.1f} F)".format(humidity, temperature, f))
        return humidity, temperature
    else:
        print("Data not good, skip")
        return -3


if __name__ =='__main__':
    humidity, temperature = main(dhtpin)