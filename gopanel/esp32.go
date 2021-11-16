package gopanel

import (
	"encoding/binary"
	"image"
	"image/color"
	"net"
)

// To565 converts a RGB888 color to RGB565 and returns in uint16 format
func To565(r, g, b uint8) uint16 {
	var result uint16
	result += (uint16(r) & 0xf8) << 8
	result += (uint16(g) & 0xfc) << 3
	result += uint16(b) >> 3
	return result
}

// SendImage packs the selected image and send through udp to a specified address
// Supports NRGBA and RGBA images
func SendImage(img image.Image, conn *net.UDPConn, addr *net.UDPAddr) {
	var buff []byte

	for y := 0; y < 64; y++ {
		pkt := make([]byte, 65*2)
		binary.LittleEndian.PutUint16(pkt, uint16(y))

		for x := 0; x < 64; x++ {
			p := img.At(x, y)
			v, ok := p.(color.NRGBA)
			k := 2 + 2*x
			if ok {
				binary.LittleEndian.PutUint16(pkt[k:], To565(v.R, v.G, v.B))
			} else {
				v := p.(color.RGBA)
				binary.LittleEndian.PutUint16(pkt[k:], To565(v.R, v.G, v.B))
			}
		}
		buff = append(buff, pkt...)

		if y%8 == 7 {
			_, _ = conn.WriteToUDP(buff, addr)
			buff = nil
		}
	}
}
