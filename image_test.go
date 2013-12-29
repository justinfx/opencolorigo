package ocio

import (
    "github.com/gographics/imagick/imagick"
    "testing"
)

func BenchmarkFloatPixels(b *testing.B) {
    infile := `/Users/justin/Pictures/apple.png`

    cfg, _ := CurrentConfig()
    ct, _ := cfg.CurrentContext()
    proc, _ := cfg.Processor(ct, "linear", "Cineon")

    wand := imagick.NewMagickWand()
    defer wand.Destroy()

    if err := wand.ReadImage(infile); err != nil {
        b.Errorf("Error reading image %q: %s", infile, err.Error())
    }

    var (
        colNum int
        rowNum int
        pixel  *imagick.PixelWand
        pixels []float64
    )

    b.ResetTimer()
    b.StartTimer()

    for i := 0; i < b.N; i++ {

        cols := int(wand.GetImageWidth())
        rows := int(wand.GetImageHeight())
        pixels = make([]float64, cols*rows*3)

        pixIter := wand.NewPixelIterator()
        defer pixIter.Destroy()

        for rowNum = 0; rowNum < rows; rowNum++ {
            for colNum, pixel = range pixIter.GetNextIteratorRow() {
                pixels[rowNum+colNum] = pixel.GetRed()
                pixels[rowNum+colNum+1] = pixel.GetGreen()
                pixels[rowNum+colNum+2] = pixel.GetBlue()
            }
        }

    }

    b.StopTimer()

    b.Log(proc)
    b.Logf("Num pixels: %d\n", len(pixels))
    b.Log(pixels[:50])
}
