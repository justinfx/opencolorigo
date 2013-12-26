package opencolorigo

import (
    "io/ioutil"
    "os"
    "testing"
)

var CONFIG *Config

func init() {
    var err error
    CONFIG, err = ConfigCreateFromData(OCIO_CONFIG)
    if err != nil {
        panic(err)
    }
}

// Global
func TestClearAllCaches(t *testing.T) {
    ClearAllCaches()
}

func TestGetVersion(t *testing.T) {
    t.Log(GetVersion())
}

func TestGetVersionHex(t *testing.T) {
    t.Log(GetVersionHex())
}

// Config
func TestGetCurrentConfig(t *testing.T) {
    c, err := GetCurrentConfig()
    if err != nil {
        t.Error(err)
        return
    }
    t.Logf("Config: %+v", c)
}

func TestConfigGetFromEnv(t *testing.T) {
    c, err := ConfigCreateFromEnv()
    if err != nil {
        t.Error(err)
        return
    }
    t.Logf("Config: %+v", c)
}

func TestConfigGetFromFile(t *testing.T) {
    c, fname, err := getConfigFromFile()
    defer os.Remove(fname)

    if err != nil {
        t.Error(err)
    }

    t.Logf("Config read from temp file %s (%v)", fname, c)
}

func TestConfigGetFromData(t *testing.T) {
    c, err := ConfigCreateFromData(OCIO_CONFIG)
    if err != nil {
        t.Error(err)
        return
    }
    t.Logf("Config: %+v", c)
}

func TestConfigGetCacheID(t *testing.T) {
    c, _ := GetCurrentConfig()
    id, err := c.GetCacheID()
    if err != nil {
        t.Error(err)
        return
    }
    t.Log(id)
}

func TestConfigGetDescription(t *testing.T) {
    d, err := CONFIG.GetDescription()
    if err != nil {
        t.Error(err)
        return
    }
    t.Log(d)
}

func TestConfigGetSearchPath(t *testing.T) {
    p, err := CONFIG.GetSearchPath()
    if err != nil {
        t.Error(err)
        return
    }
    t.Log(p)
}

func TestConfigGetWorkingDir(t *testing.T) {
    p, err := CONFIG.GetWorkingDir()
    if err != nil {
        t.Error(err)
        return
    }
    t.Log(p)
}

func TestConfigGetNumColorSpaces(t *testing.T) {
    n := CONFIG.GetNumColorSpaces()
    if n <= 0 {
        t.Error("Expected number of colorspaces to be greater than 0")
        return
    }
    t.Log(n)
}

func TestConfigGetColorSpaceNameByIndex(t *testing.T) {
    c := CONFIG

    num := c.GetNumColorSpaces()

    if num > 0 {
        var names []string
        for i := 0; i < num; i++ {
            s, err := c.GetColorSpaceNameByIndex(i)
            if err != nil {
                t.Error(err)
                return
            }
            names = append(names, s)
        }
        t.Logf("ColorSpace names: %v", names)
    }
}

func TestConfigGetIndexForColorSpace(t *testing.T) {
    c := CONFIG

    num := c.GetNumColorSpaces()
    if num <= 0 {
        t.Error("Expected number of colorspaces to be greater than 0")
        return
    }

    var (
        name string
        idx  int
        err  error
    )
    if num > 0 {
        for i := 0; i < num; i++ {
            name, err = c.GetColorSpaceNameByIndex(i)
            if err != nil {
                t.Error(err)
                return
            }

            idx, err = c.GetIndexForColorSpace(name)
            if err != nil {
                t.Error(err)
                return
            }

            if idx != i {
                t.Errorf("Expected %d for colorspace %s, got %d", i, name, idx)
                return
            }
        }
    }
}

func TestConfigIsStrictParsingEnabled(t *testing.T) {
    t.Log(CONFIG.IsStrictParsingEnabled())
}

func TestConfigSetStrictParsingEnabled(t *testing.T) {
    c := CONFIG

    orig := c.IsStrictParsingEnabled()

    err := c.SetStrictParsingEnabled(!orig)
    if err != nil {
        t.Error(err)
    }

    if c.IsStrictParsingEnabled() == orig {
        t.Errorf("Expected %v, got %v", !orig, orig)
        return
    }

    err = c.SetStrictParsingEnabled(orig)
    if err != nil {
        t.Error(err)
        return
    }

    if c.IsStrictParsingEnabled() != orig {
        t.Errorf("Expected %v, got %v", orig, !orig)
        return
    }
}

func TestRoles(t *testing.T) {
    c := CONFIG

    origCount := c.GetNumRoles()
    if origCount <= 0 {
        t.Error("Expected number of roles to be greater than 0")
        return
    }

    role := `__unittest_role__`

    space, err := c.GetColorSpaceNameByIndex(0)
    if err != nil {
        t.Error(err)
        return
    }

    err = c.SetRole(role, space)
    if err != nil {
        t.Error(err)
        return
    }

    if count := c.GetNumRoles(); count != (origCount + 1) {
        t.Errorf("Expected number of roles to be %d, but got %d", origCount+1, count)
        return
    }

    if !c.HasRole(role) {
        t.Errorf("Expected config to have the role %v", role)
        return
    }

    found := false
    for i := 0; i < c.GetNumRoles(); i++ {
        name, _ := c.GetRoleName(i)
        if name == role {
            found = true
            break
        }
    }
    if !found {
        t.Errorf("Expected to find role name %v in list of roles", role)
        return
    }

    err = c.SetRole(role, "")
    if err != nil {
        t.Error(err)
        return
    }
    if count := c.GetNumRoles(); count != origCount {
        t.Errorf("Expected number of roles to be %d, but got %d", origCount, count)
        return
    }

    if c.HasRole(role) {
        t.Errorf("Expected config to not have the role %v", role)
        return
    }
}

func getConfigFromFile() (*Config, string, error) {
    tmpfile, err := ioutil.TempFile("", "ocio_config_unittest_")
    if err != nil {
        return nil, "", err
    }

    name := tmpfile.Name()

    tmpfile.WriteString(OCIO_CONFIG)
    tmpfile.Close()

    c, err := ConfigCreateFromFile(name)
    return c, name, err
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
