import time
import network

from machine import Pin

print('Connting to WiFi')
stf_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.connect('sarmistha', 'Xs28@pr24')

while sta_if.isconnected():
    print('.')
    
print('connected')

led = Pin(12, Pin.OUT)
led.high()