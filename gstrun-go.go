package main

import (
	"fmt"
	"os"

	"github.com/ziutek/glib"
	"github.com/ziutek/gst"
)

type ctx struct {
	pipe *gst.Pipeline
	bus  *gst.Bus
}

func (ctx *ctx) onMessage(bus *gst.Bus, msg *gst.Message) {
	switch msg.GetType() {
	case gst.MESSAGE_EOS:
		ctx.pipe.SetState(gst.STATE_NULL)
		os.Exit(0)
	case gst.MESSAGE_ERROR:
		ctx.pipe.SetState(gst.STATE_NULL)
		err, debug := msg.ParseError()
		fmt.Println("Error: " + err.Error())
		fmt.Println("(Debug details: " + debug + ")")
		os.Exit(1)
	}
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: gstrun-go 'pipeline as one arg'")
		os.Exit(1)
	}

	pl, err := gst.ParseLaunch(os.Args[1])
	if err != nil {
		fmt.Println("Parse error: " + err.Error())
		os.Exit(1)
	}

	bus := pl.GetBus()

	ctx := ctx{
		pipe: pl,
		bus:  bus,
	}

	bus.AddSignalWatch()
	bus.Connect("message", ctx.onMessage, nil)

	pl.SetState(gst.STATE_PLAYING)
	glib.NewMainLoop(nil).Run()
}
