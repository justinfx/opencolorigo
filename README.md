# OpenColorIO bindings for Go

[![Go CI](https://github.com/justinfx/opencolorigo/workflows/Go/badge.svg)](https://github.com/justinfx/opencolorigo/actions?query=branch%3Amaster)
[![Go project version](https://img.shields.io/github/v/release/justinfx/opencolorigo.svg)](https://pkg.go.dev/github.com/justinfx/opencolorigo?tab=versions) 
[![GoDoc](http://godoc.org/github.com/justinfx/opencolorigo?status.svg)](https://pkg.go.dev/github.com/justinfx/opencolorigo?tab=doc) 
[![Go Report](https://goreportcard.com/badge/github.com/justinfx/opencolorigo)](https://goreportcard.com/badge/github.com/justinfx/opencolorigo)

OpenColorIO (OCIO) is a complete color management solution geared towards motion picture production with an emphasis on 
visual effects and computer animation. OCIO provides a straightforward and consistent user experience across all 
supporting applications while allowing for sophisticated back-end configuration options suitable for high-end production 
usage. OCIO is compatible with the Academy Color Encoding Specification (ACES) and is LUT-format agnostic, supporting 
many popular formats.

OpenColorIO is released as version 1.0 and has been in development since 2003. OCIO represents the culmination of years 
of production experience earned on such films as SpiderMan 2 (2004), Surf’s Up (2007), Cloudy with a Chance of Meatballs 
(2009), Alice in Wonderland (2010), and many more. OpenColorIO is natively supported in commercial applications like 
Katana, Mari, Silhouette FX, and others coming soon.

OpenColorIO is free and is one of several open source projects actively sponsored by Sony Imageworks.

http://opencolorio.org

## Requirements

* [OpenColorIO](http://opencolorio.org/)


## Status

So far only parts of the API have been exposed. Most of the Config API is done, along with the ColorSpace, Context, 
Transform, and color processing via CPU Path. 

* Implement all of the API
  * [Config/Luma](http://opencolorio.org/developers/api/OpenColorIO.html#luma)
  * [Config/Look](http://opencolorio.org/developers/api/OpenColorIO.html#look)
  * [ColorSpace/Data](http://opencolorio.org/developers/api/OpenColorIO.html#data)
  * [ColorSpace/Allocation](http://opencolorio.org/developers/api/OpenColorIO.html#allocation)
  * [Look](http://opencolorio.org/developers/api/OpenColorIO.html#look-section)
  * [Processor/GPU Path](http://opencolorio.org/developers/api/OpenColorIO.html#gpu-path) (CPU Path done)
  * [Baker](http://opencolorio.org/developers/api/OpenColorIO.html#baker)
  * [PlanarImageDesc](http://opencolorio.org/developers/api/OpenColorIO.html#planarimagedesc)
  * [GpuShaderDesc](http://opencolorio.org/developers/api/OpenColorIO.html#gpushaderdesc)


## Installation

    go get github.com/justinfx/opencolorigo

If you have installed OpenColorIO to a custom location, you will need to tell CGO where to find the 
headers and libs, and use the `no_pkgconfig` tag to disable pkg-config:

    export CGO_CPPFLAGS="-I/path/to/include"
    export CGO_LDFLAGS="-L/path/to/lib"
    go get -tags no_pkgconfig github.com/justinfx/opencolorigo

Or just prefix the `install` command, and use the `no_pkgconfig` tag:

    /usr/bin/env \
      CGO_CPPFLAGS="-I/usr/local/include" \
      CGO_LDFLAGS="-L/usr/local/lib -lopencolorio" \
      go get -tags no_pkgconfig github.com/justinfx/openimageigo

## Example

```go
func Example() {

    // Arbitrary source of image data
    //
    // ColorData is a []float32 containing the pixel values.
    // Could be in various formats:
    //     R,G,B,R,G,B,...     // 3 Channels
    //     R,G,B,A,R,G,B,A,... // 4 Channels
    //
    var imageData ocio.ColorData = getExampleImage()

    // Get the global OpenColorIO config
    // This will auto-initialize (using $OCIO) on first use
    cfg, err := ocio.CurrentConfig()
    if err != nil {
        panic(err.Error()
    }
    defer cfg.Destroy()

    // Get the processor corresponding to this transform.
    processor, err := cfg.Processor("linear", "Cineon")
    if err != nil {
        panic(err.Error())
    }
    defer processor.Destroy()

    // Wrap the image in a light-weight ImageDesc,
    // providing the width, height, and number of color channels
    // that imageData represents.
    imgDesc := ocio.NewPackedImageDesc(imageData, 512, 256, 3)
    defer imgDesc.Destroy()

    // Apply the color transformation (in place)
    err = processor.Apply(imgDesc)
    if err != nil {
        panic(err.Error())
    }
}
```

## Memory Management

The objects returned by the API are wrappers over C/C++ libopencolorio memory. While finalizers are defined
on these types in order to release C memory as some point, there isn't a guarantee as to exactly when finalizers will be run
by the garbage collector. To ensure resources are freed quickly, a direct call to the `Destroy()` method of an
API instance will immediately free the memory.
