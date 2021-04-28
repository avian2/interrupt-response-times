from machine import Pin

pin = Pin(14, Pin.IN)
pout = Pin(15, Pin.OUT)

def handler(p):
	pout.value(pin.value())

pin.irq(handler)
