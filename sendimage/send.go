package main

import (
	"fmt"
	"github.com/alecthomas/kong"
	"github.com/disintegration/imaging"
	"github.com/racerxdl/esp32-hub-panel/gopanel"
	"image"
	"image/draw"
	"net"
	"os"
	"time"
)

func loadImage(path string) (image.Image, error) {
	f, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer f.Close()

	img, _, err := image.Decode(f)
	if err != nil {
		return nil, err
	}
	b := img.Bounds()
	rgbaImg := image.NewRGBA(img.Bounds())
	draw.Draw(rgbaImg, rgbaImg.Bounds(), img, b.Min, draw.Src)

	return rgbaImg, nil
}

var CLI struct {
	Repeat int    `help:"Number of times to repeat the image"`
	Delay  int    `help:"Delay in ms between repeat cycles"`
	Path   string `arg:"" name:"path" help:"Path of the image" type:"path"`
	Host   string `arg:"" name:"host" help:"Hostname of the panel"`
}

func main() {
	_ = kong.Parse(&CLI)
	fmt.Printf("sending image %s to %s\n", CLI.Path, CLI.Host)

	img, err := loadImage(CLI.Path)
	if err != nil {
		panic(err)
	}

	addr, err := net.ResolveUDPAddr("udp4", fmt.Sprintf("%s:1234", CLI.Host))
	if err != nil {
		panic(err)
	}
	packetConn, err := net.ListenPacket("udp", ":40000")
	conn := packetConn.(*net.UDPConn)
	img = imaging.AdjustBrightness(img, 0)
	CLI.Repeat += 2 // Send at least twice

	delay := time.Millisecond * time.Duration(CLI.Delay)
	if CLI.Delay == 0 {
		delay = time.Millisecond * 10
	}

	for CLI.Repeat > 0 {
		gopanel.SendImage(img, conn, addr)
		CLI.Repeat--
		time.Sleep(delay)
	}
}
