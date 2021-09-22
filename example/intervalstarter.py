from micropython import const
from machine import Timer, Pin
from time import sleep_ms
from builtindac import tone, init
import utime

COUNTDOWN_MODE_15 = const(15)
COUNTDOWN_MODE_30 = const(30)
COUNTDOWN_MODE_60 = const(60)
COUNTDOWN_MODE_90 = const(90)
COUNTDOWN_CYCLE_SECONDS = const(180)

COUNTDOWN_30 = const(30)
COUNTDOWN_20 = const(20)
COUNTDOWN_10 = const(10)
COUNTDOWN_05 = const(5)
COUNTDOWN_00 = const(0)

GPIO_PLUS_30 = const(27) #GPIO_NUM_27
GPIO_PLUS_60 = const(14) #GPIO_NUM_14

global nextIntervalCounter
global nextCountdown

timer0 = Timer(0)
pinPLUS30 = Pin(GPIO_PLUS_30, Pin.IN, Pin.PULL_UP)
pinPLUS60 = Pin(GPIO_PLUS_60, Pin.IN, Pin.PULL_UP)

def getIntervalSeconds():
    c = pinPLUS30.value() + pinPLUS60.value()*2
    if c==0:
        return COUNTDOWN_MODE_90
    elif c==1:
        return COUNTDOWN_MODE_60
    elif c==2:
        return COUNTDOWN_MODE_30
    else:
        return COUNTDOWN_MODE_15

def ontimer(timer):
    global nextIntervalCounter
    global nextCountdown

    ticks = utime.ticks_ms()

    # tone
    if (nextCountdown==COUNTDOWN_00):
        tone(1760, 750) # A6 ラ6
    elif (nextCountdown<=COUNTDOWN_05) and (nextCountdown>COUNTDOWN_00):
        tone( 880, 250) # A5 ラ5
    elif (nextCountdown==COUNTDOWN_10):
        tone( 880, 500) # A5 ラ5
    elif (nextCountdown==COUNTDOWN_20):
        tone( 880, 250) # A5 ラ5
    elif (nextCountdown==COUNTDOWN_30):
        tone( 880, 250) #A5 ラ5
        tone(1760, 250) #A6 ラ6

    print("[{}] Starting in {} seconds...".format(ticks, nextCountdown))
    
    # next
    nextIntervalCounter = (nextIntervalCounter + 1) % COUNTDOWN_CYCLE_SECONDS
    nextCountdown = (COUNTDOWN_CYCLE_SECONDS-nextIntervalCounter) % getIntervalSeconds()

def starttimer():
    global nextIntervalCounter
    global nextCountdown
    stoptimer()
    nextIntervalCounter = COUNTDOWN_CYCLE_SECONDS - COUNTDOWN_10
    nextCountdown = COUNTDOWN_10
    ontimer(None)
    timer0.init(period=1000, mode=Timer.PERIODIC, callback=ontimer)

def stoptimer():
    timer0.deinit()

def run_loop():
    starttimer()
    try:
        while(1):
            sleep_ms(500)
    finally:
        stoptimer()

if __name__ == "__main__":
    run_loop()
