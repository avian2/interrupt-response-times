import time
import rp2
from machine import Pin

@rp2.asm_pio(set_init=rp2.PIO.OUT_LOW)
def handle():
	wrap_target()
	wait(1, pin, 0)
	set(pins, 1)
	wait(0, pin, 0)
	set(pins, 0)
	wrap()

sm = rp2.StateMachine(0, handle, in_base=Pin(14), set_base=Pin(15))
sm.active(1)
