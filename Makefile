# Compilers m shet
CC?=gcc
GO?=go

# Versions
GST_PKG=gstreamer-1.0

# Flags!
GST_CFLAGS=$(shell pkg-config --cflags --libs $(GST_PKG))

# Rules
gstrun: gstrun.c
	$(CC) $(GST_CFLAGS) -o $@ $<

gstrun-go: gstrun-go.go
	$(GO) build -o $@ $<

# ALL!!!
all: gstrun gstrun-go
