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

func TestVersion(t *testing.T) {
    t.Log(Version())
}

func TestVersionHex(t *testing.T) {
    t.Log(VersionHex())
}

func TestLoggingLevel(t *testing.T) {
    original := LoggingLevel()
    defer SetLoggingLevel(original)

    t.Logf("Current logging level: %v", original)

    levels := []int{
        LOGGING_LEVEL_NONE,
        LOGGING_LEVEL_DEBUG,
        LOGGING_LEVEL_INFO,
        LOGGING_LEVEL_WARNING,
    }

    for _, level := range levels {
        SetLoggingLevel(level)
        actual := LoggingLevel()
        if level != actual {
            t.Errorf("Execpted logging level %d, but got %d", level, actual)
        }
    }
}

// Config
func TestCreateConfig(t *testing.T) {
    c := NewConfig()

    num := c.NumColorSpaces()
    if num != 0 {
        t.Errorf("Expected number of colorspaces to be 0, got %d", num)
    }
}

func TestCurrentConfig(t *testing.T) {
    c, err := CurrentConfig()
    defer SetCurrentConfig(c)

    if err != nil {
        t.Error(err.Error())
        return
    }
    t.Logf("Config: %+v", c)

    err = SetCurrentConfig(CONFIG)
    if err != nil {
        t.Error(err.Error())
    }
}

func TestConfigFromEnv(t *testing.T) {
    c, err := ConfigCreateFromEnv()
    if err != nil {
        t.Error(err.Error())
        return
    }
    t.Logf("Config: %+v", c)
}

func TestConfigFromFile(t *testing.T) {
    c, fname, err := getConfigFromFile()
    defer os.Remove(fname)

    if err != nil {
        t.Error(err.Error())
    }

    t.Logf("Config read from temp file %s (%v)", fname, c)
}

func TestConfigFromData(t *testing.T) {
    c, err := ConfigCreateFromData(OCIO_CONFIG)
    if err != nil {
        t.Error(err.Error())
        return
    }
    t.Logf("Config: %+v", c)
}

func TestConfigSerialize(t *testing.T) {
    ser, err := CONFIG.Serialize()
    if err != nil {
        t.Error(err.Error())
        return
    }
    if ser == "" {
        t.Error("Serialized config string is empty")
    }
}

func TestConfigEditableCopy(t *testing.T) {
    c_copy := CONFIG.EditableCopy()
    t.Logf("Config: %+v is a copy of Config: %+v", c_copy, CONFIG)
}

func TestConfigSanityCheck(t *testing.T) {
    if err := CONFIG.SanityCheck(); err != nil {
        t.Error(err.Error())
    }
}

func TestConfigCacheID(t *testing.T) {
    c, _ := CurrentConfig()

    id, err := c.CacheID()
    if err != nil {
        t.Error(err.Error())
    } else {
        t.Log(id)
    }

    id, err = c.CacheIDWithContext(nil)
    if err != nil {
        t.Error(err.Error())
    } else {
        t.Log(id)
    }

    context, _ := c.CurrentContext()
    id, err = c.CacheIDWithContext(context)
    if err != nil {
        t.Error(err.Error())
    } else {
        t.Log(id)
    }
}

func TestConfigDescription(t *testing.T) {
    d, err := CONFIG.Description()
    if err != nil {
        t.Error(err.Error())
        return
    }
    t.Log(d)
}

func TestConfigCurrentContext(t *testing.T) {
    c, _ := CurrentConfig()
    p, err := c.CurrentContext()
    if err != nil {
        t.Error(err.Error())
        return
    }
    t.Logf("Current Context: %+v", p)
}

func TestConfigSearchPath(t *testing.T) {
    p, err := CONFIG.SearchPath()
    if err != nil {
        t.Error(err.Error())
        return
    }
    t.Log(p)
}

func TestConfigWorkingDir(t *testing.T) {
    p, err := CONFIG.WorkingDir()
    if err != nil {
        t.Error(err.Error())
        return
    }
    t.Log(p)
}

func TestConfigNumColorSpaces(t *testing.T) {
    n := CONFIG.NumColorSpaces()
    if n <= 0 {
        t.Error("Expected number of colorspaces to be greater than 0")
        return
    }
    t.Log(n)
}

func TestConfigColorSpaceNameByIndex(t *testing.T) {
    c := CONFIG

    num := c.NumColorSpaces()

    if num > 0 {
        var names []string
        for i := 0; i < num; i++ {
            s, err := c.ColorSpaceNameByIndex(i)
            if err != nil {
                t.Error(err.Error())
                return
            }
            names = append(names, s)
        }
        t.Logf("ColorSpace names: %v", names)
    }
}

func TestConfigIndexForColorSpace(t *testing.T) {
    c := CONFIG

    num := c.NumColorSpaces()
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
            name, err = c.ColorSpaceNameByIndex(i)
            if err != nil {
                t.Error(err.Error())
                return
            }

            idx, err = c.IndexForColorSpace(name)
            if err != nil {
                t.Error(err.Error())
                return
            }

            if idx != i {
                t.Errorf("Expected %d for colorspace %s, got %d", i, name, idx)
                return
            }
        }
    }
}

func TestConfigStrictParsingEnabled(t *testing.T) {
    c := CONFIG
    orig := c.IsStrictParsingEnabled()

    err := c.SetStrictParsingEnabled(!orig)
    if err != nil {
        t.Error(err.Error())
    }

    if c.IsStrictParsingEnabled() == orig {
        t.Errorf("Expected %v, got %v", !orig, orig)
        return
    }

    err = c.SetStrictParsingEnabled(orig)
    if err != nil {
        t.Error(err.Error())
        return
    }

    if c.IsStrictParsingEnabled() != orig {
        t.Errorf("Expected %v, got %v", orig, !orig)
        return
    }
}

func TestRoles(t *testing.T) {
    c := CONFIG

    origCount := c.NumRoles()
    if origCount <= 0 {
        t.Error("Expected number of roles to be greater than 0")
        return
    }

    role := `__unittest_role__`

    space, err := c.ColorSpaceNameByIndex(0)
    if err != nil {
        t.Error(err.Error())
        return
    }

    err = c.SetRole(role, space)
    if err != nil {
        t.Error(err.Error())
        return
    }

    if count := c.NumRoles(); count != (origCount + 1) {
        t.Errorf("Expected number of roles to be %d, but got %d", origCount+1, count)
        return
    }

    if !c.HasRole(role) {
        t.Errorf("Expected config to have the role %v", role)
        return
    }

    found := false
    for i := 0; i < c.NumRoles(); i++ {
        name, _ := c.RoleName(i)
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
        t.Error(err.Error())
        return
    }
    if count := c.NumRoles(); count != origCount {
        t.Errorf("Expected number of roles to be %d, but got %d", origCount, count)
        return
    }

    if c.HasRole(role) {
        t.Errorf("Expected config to not have the role %v", role)
        return
    }
}

func TestColorSpace(t *testing.T) {
    c := CONFIG

    name, _ := c.ColorSpaceNameByIndex(0)
    cs, err := c.ColorSpace(name)
    if err != nil {
        t.Errorf("Error getting a ColorSpace from name %s: %s", name, err.Error())
        return
    }
    t.Logf("ColorSpace: %+v", cs)
}

/*

ColorSpaces

*/
func TestColorSpaceCreate(t *testing.T) {
    cs := NewColorSpace()
    t.Log(cs)
}

func TestColorSpaceEditableCopy(t *testing.T) {
    cs, _ := CONFIG.ColorSpace("linear")
    cs_copy := cs.EditableCopy()
    t.Logf("%s is a copy of %s", cs_copy, cs)

    if cs.Name() != cs_copy.Name() {
        t.Errorf("Copy colorspace name is %s, but expected %s", cs_copy.Name(), cs.Name())
    }
}

func TestColorSpaceName(t *testing.T) {
    cs, _ := CONFIG.ColorSpace("linear")
    cs.SetName("FOO")
    defer cs.SetName("linear")

    if cs.Name() != "FOO" {
        t.Errorf("Expected ColorSpace name to be FOO, got %s", cs.Name())
        return
    }
}

func TestColorSpaceFamily(t *testing.T) {
    cs, _ := CONFIG.ColorSpace("linear")
    family := cs.Family()
    cs.SetFamily("FOO")
    defer cs.SetFamily(family)

    if cs.Family() != "FOO" {
        t.Errorf("Expected ColorSpace family to be FOO, got %s", cs.Family())
        return
    }
}

func TestColorSpaceEqualityGroup(t *testing.T) {
    cs, _ := CONFIG.ColorSpace("linear")
    group := cs.EqualityGroup()
    cs.SetEqualityGroup("FOO")
    defer cs.SetEqualityGroup(group)

    if cs.EqualityGroup() != "FOO" {
        t.Errorf("Expected ColorSpace EqualityGroup to be FOO, got %s", cs.EqualityGroup())
        return
    }
}

func TestColorSpaceDescription(t *testing.T) {
    cs, err := CONFIG.ColorSpace("linear")
    if err != nil {
        t.Error(err.Error())
        return
    }
    desc := cs.Description()
    cs.SetDescription("FOO")
    defer cs.SetDescription(desc)

    if cs.Description() != "FOO" {
        t.Errorf("Expected ColorSpace Description to be FOO, got %s", cs.Description())
        return
    }
}

func TestColorSpaceBitDepth(t *testing.T) {
    cs, err := CONFIG.ColorSpace("linear")
    if err != nil {
        t.Error(err.Error())
        return
    }

    depth := cs.BitDepth()
    defer cs.SetBitDepth(depth)

    depths := []int{
        BIT_DEPTH_UNKNOWN,
        BIT_DEPTH_UINT8,
        BIT_DEPTH_UINT10,
        BIT_DEPTH_UINT12,
        BIT_DEPTH_UINT14,
        BIT_DEPTH_UINT16,
        BIT_DEPTH_UINT32,
        BIT_DEPTH_F16,
        BIT_DEPTH_F32,
    }

    for _, d := range depths {
        cs.SetBitDepth(d)
        if cs.BitDepth() != d {
            t.Errorf("Expected ColorSpace BitDepth to be %v, got %v", d, cs.BitDepth())
            return
        }
    }
}

/*

Context

*/

func TestContextCreate(t *testing.T) {
    c := NewContext()
    t.Logf("New Context: %+v", c)
}

func TestContextEditableCopy(t *testing.T) {
    c := NewContext()
    c.SetStringVar("FOO", "BAR")
    c_copy := c.EditableCopy()

    if c_copy.StringVar("FOO") != "BAR" {
        t.Errorf("Expected FOO=BAR, got %s", c_copy.StringVar("FOO"))
    }

}

func TestContextCacheID(t *testing.T) {
    c, _ := CurrentConfig()
    context, _ := c.CurrentContext()

    id, err := context.CacheID()
    if err != nil {
        t.Error(err.Error())
        return
    }
    t.Log(id)
}

func TestContextSearchPath(t *testing.T) {
    c, _ := CurrentConfig()
    context, _ := c.CurrentContext()

    orig := context.SearchPath()
    defer context.SetSearchPath(orig)

    expected := "/FOO/BAR"
    context.SetSearchPath(expected)
    actual := context.SearchPath()

    if actual != expected {
        t.Errorf("Expected search path to be %q, got %q", expected, actual)
    }
}

func TestContextWorkingDir(t *testing.T) {
    c, _ := CurrentConfig()
    context, _ := c.CurrentContext()

    orig := context.WorkingDir()
    defer context.SetWorkingDir(orig)

    expected := "/FOO/BAR"
    context.SetWorkingDir(expected)
    actual := context.WorkingDir()

    if actual != expected {
        t.Errorf("Expected working dir to be %q, got %q", expected, actual)
    }
}

func TestContextLoadEnvironment(t *testing.T) {
    c := NewContext()
    c.LoadEnvironment()
}

/*

Utility

*/
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
