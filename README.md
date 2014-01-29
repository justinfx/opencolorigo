# OpenColorIO bindings for Go

OpenColorIO (OCIO) is a complete color management solution geared towards motion picture production with an emphasis on visual effects and computer animation. OCIO provides a straightforward and consistent user experience across all supporting applications while allowing for sophisticated back-end configuration options suitable for high-end production usage. OCIO is compatible with the Academy Color Encoding Specification (ACES) and is LUT-format agnostic, supporting many popular formats.

OpenColorIO is released as version 1.0 and has been in development since 2003. OCIO represents the culmination of years of production experience earned on such films as SpiderMan 2 (2004), Surf’s Up (2007), Cloudy with a Chance of Meatballs (2009), Alice in Wonderland (2010), and many more. OpenColorIO is natively supported in commercial applications like Katana, Mari, Silhouette FX, and others coming soon.

OpenColorIO is free and is one of several open source projects actively sponsored by Sony Imageworks.

http://opencolorio.org

Requirements
----------------------

* [OpenColorIO](http://opencolorio.org/)


Status
---------

So far only parts of the API have been exposed. Most of the Config API is done, along with the ColorSpace, Context, and color processing via CPU Path. 

* Implement all of the API
  * [Config/Luma](http://opencolorio.org/developers/api/OpenColorIO.html#luma)
  * [Config/Look](http://opencolorio.org/developers/api/OpenColorIO.html#look)
  * [ColorSpace/Data](http://opencolorio.org/developers/api/OpenColorIO.html#data)
  * [ColorSpace/Allocation](http://opencolorio.org/developers/api/OpenColorIO.html#allocation)
  * [ColorSpace/Transform](http://opencolorio.org/developers/api/OpenColorIO.html#transform)
  * [Look](http://opencolorio.org/developers/api/OpenColorIO.html#look-section)
  * [Processor/GPU Path](http://opencolorio.org/developers/api/OpenColorIO.html#gpu-path) (CPU Path done)
  * [Baker](http://opencolorio.org/developers/api/OpenColorIO.html#baker)
  * [PlanarImageDesc](http://opencolorio.org/developers/api/OpenColorIO.html#planarimagedesc)
  * [GpuShaderDesc](http://opencolorio.org/developers/api/OpenColorIO.html#gpushaderdesc)


Installation
------------

    go get github.com/justinfx/opencolorigo

Documentation
-------------

[http://godoc.org/github.com/justinfx/opencolorigo](http://godoc.org/github.com/justinfx/opencolorigo)


Example
-------

```
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

    // Get the processor corresponding to this transform.
    processor, err := cfg.Processor("linear", "Cineon")
    if err != nil {
        panic(err.Error())
    }

    // Wrap the image in a light-weight ImageDesc,
    // providing the width, height, and number of color channels
    // that imageData represents.
    imgDesc := ocio.NewPackedImageDesc(imageData, 512, 256, 3)

    // Apply the color transformation (in place)
    err = processor.Apply(imgDesc)
    if err != nil {
        panic(err.Error())
    }
}
```
