# OpenColorIO bindings for Go

This package is a work-in-progress
----------------------------------

So far only bits and pieces of the API have been exposed. It's a starting point for anyone else that wants to jump in and contribute.

*TODO*

* More fine-grained error Handling
* Implement all of the API


Requirements
----------------------

* [OpenColorIO](http://opencolorio.org/)


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
        fmt.Errorf("Error getting the current config: %s\n", err.Error())
        return
    }

    // Get the processor corresponding to this transform.
    processor, err := cfg.Processor("linear", "Cineon")
    if err != nil {
        fmt.Errorf("Error building the processor with given values: %s\n", err.Error())
        return
    }

    // Wrap the image in a light-weight ImageDesc,
    // providing the width, height, and number of color channels
    // that imageData represents.
    imgDesc := ocio.NewPackedImageDesc(imageData, 512, 256, 3)

    // Apply the color transformation (in place)
    err = processor.Apply(imgDesc)
    if err != nil {
        fmt.Errorf("Error applying the color transformation to image: %s\n", err.Error())
    }
}
```
