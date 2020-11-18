# Connections

# POWER
## GND  Green --> GND #1
## 3.3V Blue  --> 3V3

# LEDs
## GREEN LED Green  --> pin 3
## AMBER LED Yellow --> pin 4
## RED   LED Orange --> pin 5

## BUZZER

### Blue/purple --> pin 6

## LCD

### Red --> 3V
### Brown --> GND #1
### Orange --> micro:bit pin 0 which is tx-pin 3 in the overlay. rx-pin set to 16.


UART config
-----------

In bbc_microbit.overlay

&uart0 {
   status = "ok";
   compatible = "nordic,nrf-uart";
   current-speed = <9600>;
   tx-pin = <3>;
   rx-pin = <16>;
};

