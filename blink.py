from machine import Pin
import time

p = Pin(1, Pin.OUT)

def toggle(max):
    lap = 0
    
    while lap < max:
        p.value(1)
        time.sleep(1)
        p.value(0)
        time.sleep(1)
        lap += 1

toggle(5) 