# NEMO

### Nemo: NMEA Equipment serial Multiplexer on ST NUCLEO

The original idea behind this project was to build a simple system that reads a boat GPS signal and retransmits this signal to an another device via a WiFi network.
Very often navigation instruments are easy to check: they are near the helm wheel. On the contrary other navigation aids like the chart plotter or the AIS are below deck.
Some solutions already exists but they tend to use a proprietary (and closed! :( ) protocol, in this way the final nerd-user has no control over the data transmitted and collected. NEMO stays on an intermediate level, totally transparent to the final user and to the other instruments: it simply collects the data and transmits them over wifi to a final device.

### How it works:
#### Protocols
A basic idea of the [NMEA 0183](https://en.wikipedia.org/wiki/NMEA_0183) (a little bit of [NMEA 2000](https://en.wikipedia.org/wiki/NMEA_2000)) and [RS 232](https://en.wikipedia.org/wiki/RS-232) protocols is required.

### Hardware
The original hardware for NEMO was an Arduino UNO board but we had to use an STM32 NUCLEO F4111RE board due to the rtos support. Because the board works on TTL logic levels (0-5V) and the RS 232 protocol works on signal with +/- 12V a signal converter is needed. I used a MAX232 integrated circuit which can converte (bidirectionally) the signal. A Wifi Module is then used to transimt the collected data.

# schema circuito


### Software
The code on the board is divided in 3 threads operating in the producer/consumer architecture. We have the 2 (or N) producer threads that read the input data from the serial ports and queue it. Finally the consumer thread reads and "consume" the queue sending the signal over a third serial port connected to a wifi module.
