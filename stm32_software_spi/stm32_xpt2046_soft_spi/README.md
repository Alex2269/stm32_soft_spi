# stm32_xpt2046_soft_spi

``` cpp
connect the relevant port and observe pressing a touchscreen of coordinates

setting tty terminal:
stty -F /dev/ttyACM0 115200 cs8 -cstopb -parity -icanon min 1 time 1
stty -F /dev/ttyUSB0 115200 cs8 -cstopb -parity -icanon min 1 time 1

receive:
cat /dev/ttyACM0
cat /dev/ttyUSB0

transmit:
echo " Hello rs232" > /dev/ttyACM0
echo " Hello rs232" > /dev/ttyUSB0
```
