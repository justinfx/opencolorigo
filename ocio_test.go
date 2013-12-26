package ocio

import (
    "fmt"
    "testing"
)

var CONFIG Config

func init() {
    CONFIG = ConfigCreateFromData(OCIO_CONFIG)
}

// Global
func TestClearAllCaches(t *testing.T) {
    ClearAllCaches()
}

func TestGetVersion(t *testing.T) {
    fmt.Println(GetVersion())
}

func TestGetVersionHex(t *testing.T) {
    fmt.Println(GetVersionHex())
}

// Config
func TestGetCurrentConfig(t *testing.T) {
    fmt.Println(GetCurrentConfig())
}

func TestConfigGetFromEnv(t *testing.T) {
    fmt.Println(ConfigCreateFromEnv())
}

func TestConfigGetFromData(t *testing.T) {
    fmt.Println(ConfigCreateFromData(OCIO_CONFIG))
}

func TestConfigGetCacheID(t *testing.T) {
    fmt.Println(CONFIG.GetCacheID())
}

func TestConfigGetDescription(t *testing.T) {
    fmt.Println(CONFIG.GetDescription())
}

func TestConfigGetSearchPath(t *testing.T) {
    fmt.Println(CONFIG.GetSearchPath())
}

func TestConfigGetWorkingDir(t *testing.T) {
    fmt.Println(CONFIG.GetWorkingDir())
}

func TestConfigGetNumColorSpaces(t *testing.T) {
    fmt.Println(CONFIG.GetNumColorSpaces())
}

func TestConfigGetColorSpaceNameByIndex(t *testing.T) {
    c := CONFIG
    num := c.GetNumColorSpaces()
    if num > 0 {
        var names []string
        for i := 0; i < num; i++ {
            names = append(names, c.GetColorSpaceNameByIndex(i))
        }
        fmt.Println(names)
    }
}

func TestConfigGetIndexForColorSpace(t *testing.T) {
    c := CONFIG
    num := c.GetNumColorSpaces()
    if num > 0 {
        for i := 0; i < num; i++ {
            name := c.GetColorSpaceNameByIndex(i)
            idx := c.GetIndexForColorSpace(name)
            if idx != i {
                t.Errorf("Expected %d for colorspace %s, got %d", i, name, idx)
            }
        }
    }
}

func TestConfigIsStrictParsingEnabled(t *testing.T) {
    fmt.Println(CONFIG.IsStrictParsingEnabled())
}

func TestConfigSetStrictParsingEnabled(t *testing.T) {
    c := CONFIG
    orig := c.IsStrictParsingEnabled()

    c.SetStrictParsingEnabled(!orig)
    if c.IsStrictParsingEnabled() == orig {
        t.Errorf("Expected %v, got %v", !orig, orig)
    }

    c.SetStrictParsingEnabled(orig)
    if c.IsStrictParsingEnabled() != orig {
        t.Errorf("Expected %v, got %v", orig, !orig)
    }
}

func TestRoles(t *testing.T) {
    c := CONFIG
    origCount := c.GetNumRoles()

    role := `__unittest_role__`
    space := c.GetColorSpaceNameByIndex(0)

    c.SetRole(role, space)
    if count := c.GetNumRoles(); count != (origCount + 1) {
        t.Errorf("Expected number of roles to be %d, but got %d", origCount+1, count)
    }

    if !c.HasRole(role) {
        t.Errorf("Expected config to have the role %v", role)
    }

    found := false
    for i := 0; i < c.GetNumRoles(); i++ {
        if c.GetRoleName(i) == role {
            found = true
            break
        }
    }
    if !found {
        t.Errorf("Expected to find role name %v in list of roles", role)
    }

    c.SetRole(role, "")
    if count := c.GetNumRoles(); count != origCount {
        t.Errorf("Expected number of roles to be %d, but got %d", origCount, count)
    }

    if c.HasRole(role) {
        t.Errorf("Expected config to not have the role %v", role)
    }
}

const OCIO_CONFIG = `
ocio_profile_version: 1

search_path: luts
strictparsing: true
luma: [0.2126, 0.7152, 0.0722]

roles:
  color_picking: sRGB
  color_timing: Cineon
  compositing_log: Cineon
  data: raw
  default: raw
  matte_paint: sRGB
  reference: linear
  scene_linear: linear
  texture_paint: sRGB

displays:
  default:
    - !<View> {name: None, colorspace: raw}
    - !<View> {name: sRGB, colorspace: sRGB}
    - !<View> {name: rec709, colorspace: rec709}

active_displays: [default]
active_views: [sRGB]

colorspaces:
  - !<ColorSpace>
    name: linear
    family: ""
    equalitygroup: ""
    bitdepth: 32f
    description: |
      Scene-linear, high dynamic range. Used for rendering and compositing.
    isdata: false
    allocation: lg2
    allocationvars: [-15, 6]

  - !<ColorSpace>
    name: sRGB
    family: ""
    equalitygroup: ""
    bitdepth: 32f
    description: |
      Standard RGB Display Space
    isdata: false
    allocation: uniform
    allocationvars: [-0.125, 1.125]
    to_reference: !<FileTransform> {src: srgb.spi1d, interpolation: linear}

  - !<ColorSpace>
    name: sRGBf
    family: ""
    equalitygroup: ""
    bitdepth: 32f
    description: |
      Standard RGB Display Space, but with additional range to preserve float highlights.
    isdata: false
    allocation: uniform
    allocationvars: [-0.125, 4.875]
    to_reference: !<FileTransform> {src: srgbf.spi1d, interpolation: linear}

  - !<ColorSpace>
    name: rec709
    family: ""
    equalitygroup: ""
    bitdepth: 32f
    description: |
      Rec. 709 (Full Range) Display Space
    isdata: false
    allocation: uniform
    allocationvars: [-0.125, 1.125]
    to_reference: !<FileTransform> {src: rec709.spi1d, interpolation: linear}

  - !<ColorSpace>
    name: Cineon
    family: ""
    equalitygroup: ""
    bitdepth: 32f
    description: |
      Cineon (Log Film Scan)
    isdata: false
    allocation: uniform
    allocationvars: [-0.125, 1.125]
    to_reference: !<FileTransform> {src: cineon.spi1d, interpolation: linear}

  - !<ColorSpace>
    name: Gamma1.8
    family: ""
    equalitygroup: ""
    bitdepth: 32f
    description: |
      Emulates a idealized Gamma 1.8 display device.
    isdata: false
    allocation: uniform
    allocationvars: [0, 1]
    to_reference: !<ExponentTransform> {value: [1.8, 1.8, 1.8, 1]}

  - !<ColorSpace>
    name: Gamma2.2
    family: ""
    equalitygroup: ""
    bitdepth: 32f
    description: |
      Emulates a idealized Gamma 2.2 display device.
    isdata: false
    allocation: uniform
    allocationvars: [0, 1]
    to_reference: !<ExponentTransform> {value: [2.2, 2.2, 2.2, 1]}

  - !<ColorSpace>
    name: Panalog
    family: ""
    equalitygroup: ""
    bitdepth: 32f
    description: |
      Sony/Panavision Genesis Log Space
    isdata: false
    allocation: uniform
    allocationvars: [-0.125, 1.125]
    to_reference: !<FileTransform> {src: panalog.spi1d, interpolation: linear}

  - !<ColorSpace>
    name: REDLog
    family: ""
    equalitygroup: ""
    bitdepth: 32f
    description: |
      RED Log Space
    isdata: false
    allocation: uniform
    allocationvars: [-0.125, 1.125]
    to_reference: !<FileTransform> {src: redlog.spi1d, interpolation: linear}

  - !<ColorSpace>
    name: ViperLog
    family: ""
    equalitygroup: ""
    bitdepth: 32f
    description: |
      Viper Log Space
    isdata: false
    allocation: uniform
    allocationvars: [-0.125, 1.125]
    to_reference: !<FileTransform> {src: viperlog.spi1d, interpolation: linear}

  - !<ColorSpace>
    name: AlexaV3LogC
    family: ""
    equalitygroup: ""
    bitdepth: 32f
    description: |
      Alexa Log C
    isdata: false
    allocation: uniform
    allocationvars: [-0.125, 1.125]
    to_reference: !<FileTransform> {src: alexalogc.spi1d, interpolation: linear}

  - !<ColorSpace>
    name: PLogLin
    family: ""
    equalitygroup: ""
    bitdepth: 32f
    description: |
      Josh Pines style pivoted log/lin conversion. 445->0.18
    isdata: false
    allocation: uniform
    allocationvars: [-0.125, 1.125]
    to_reference: !<FileTransform> {src: ploglin.spi1d, interpolation: linear}

  - !<ColorSpace>
    name: SLog
    family: ""
    equalitygroup: ""
    bitdepth: 32f
    description: |
      Sony SLog
    isdata: false
    allocation: uniform
    allocationvars: [-0.125, 1.125]
    to_reference: !<FileTransform> {src: slog.spi1d, interpolation: linear}

  - !<ColorSpace>
    name: raw
    family: ""
    equalitygroup: ""
    bitdepth: 32f
    description: |
      Raw Data. Used for normals, points, etc.
    isdata: true
    allocation: uniform
`
