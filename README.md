# esp32-hub-panel
A ESP32 clock / display using 64x32 HUB75 panels.


This is a very hacky code that will display a clock in two 64x32 HUB75 LED Matrix Panels arranged in a 64x64 matrix. It can also receive images through UDP.

If an image has not been sent to the panel in the last 5 seconds, it automatically switches back to clock mode.

# Clock Mode

![Clock Mode](clock.jpg)

# Image Mode

Use sendimage golang script:

```bash
cd sendimage
go build -o send
send image.jpg HOSTNAME.local
```

![Image Mode](sendimage.jpg)


# Packet Format

The ESP32 listens on UDP port 1234 and expects the UDP Packet to contain one or more PanelPacket structs inside it. You can send as many PanelPacket structs inside a UDP packet and the ESP32 will process them sequentially. The only limitation is that the UDP packet cannot exceed the network MTU since it does not support packet fragmentation.

```c
struct PanelPacket {
  uint16_t lineN; // Number of the line
  uint16_t pixels[64]; // Line Content
};
```
