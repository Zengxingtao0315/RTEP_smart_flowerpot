import RPi.GPIO as GPIO

# Set GPIO mode to BCM
GPIO.setmode(GPIO.BCM)
lightpin = 18 #BCM is 17, wiring is 0, BORAD is 11

def read_pin(lightpin):
    # Set the pin to input mode and turn on the pull-up resistor
    GPIO.setup(lightpin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    # Read pin values
    value = GPIO.input(lightpin)
    if value == GPIO.LOW:
        return "Light"
    else:
        return "Dark"
