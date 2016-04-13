import RPi.GPIO as GPIO

PIN = 17
POUT = 18

GPIO.setmode(GPIO.BCM)
GPIO.setup(PIN, GPIO.IN)
GPIO.setup(POUT, GPIO.OUT)

while True:
	GPIO.wait_for_edge(PIN, GPIO.RISING)
	GPIO.output(POUT, GPIO.HIGH)
	GPIO.wait_for_edge(PIN, GPIO.FALLING)
	GPIO.output(POUT, GPIO.LOW)
