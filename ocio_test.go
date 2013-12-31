package ocio

import (
	"fmt"
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

// Usage Example: Compositing plugin that converts from “log” to “lin”
func Example() {

	// Arbitrary source of image data
	//
	// ColorData is a []float32 containing the pixel values.
	// Could be in various formats:
	//     R,G,B,R,G,B,...     // 3 Channels
	//     R,G,B,A,R,G,B,A,... // 4 Channels
	//
	var imageData ColorData = getExampleImage()

	// Get the global OpenColorIO config
	// This will auto-initialize (using $OCIO) on first use
	cfg, err := CurrentConfig()
	if err != nil {
		fmt.Errorf("Error getting the current config: %s\n", err.Error())
		return
	}

	// Get the processor corresponding to this transform.
	processor, err := cfg.Processor(ROLE_COMPOSITING_LOG, ROLE_SCENE_LINEAR)
	if err != nil {
		fmt.Errorf("Error building the processor with given values: %s\n", err.Error())
		return
	}

	// Wrap the image in a light-weight ImageDesc,
	// providing the width, height, and number of color channels
	// that imageData represents.
	imgDesc := NewPackedImageDesc(imageData, 512, 256, 3)

	// Apply the color transformation (in place)
	err = processor.Apply(imgDesc)
	if err != nil {
		fmt.Errorf("Error applying the color transformation to image: %s\n", err.Error())
	}
}

func getExampleImage() ColorData {
	return getImageData(512, 256, 3)
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

	levels := []LoggingLevelType{
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
		t.Fatal(err.Error())
	}
	t.Logf("Config: %+v", c)

	err = SetCurrentConfig(CONFIG)
	if err != nil {
		t.Fatal(err.Error())
	}
}

func TestConfigFromEnv(t *testing.T) {
	c, err := ConfigCreateFromEnv()
	if err != nil {
		t.Fatal(err.Error())
	}
	t.Logf("Config: %+v", c)
}

func TestConfigFromFile(t *testing.T) {
	c, fname, err := getConfigFromFile()
	defer os.Remove(fname)

	if err != nil {
		t.Fatal(err.Error())
	}

	t.Logf("Config read from temp file %s (%v)", fname, c)
}

func TestConfigFromData(t *testing.T) {
	c, err := ConfigCreateFromData(OCIO_CONFIG)
	if err != nil {
		t.Fatal(err.Error())
	}
	t.Logf("Config: %+v", c)
}

func TestConfigSerialize(t *testing.T) {
	ser, err := CONFIG.Serialize()
	if err != nil {
		t.Fatal(err.Error())
	}
	if ser == "" {
		t.Fatal("Serialized config string is empty")
	}
}

func TestConfigEditableCopy(t *testing.T) {
	c_copy := CONFIG.EditableCopy()
	t.Logf("Config: %+v is a copy of Config: %+v", c_copy, CONFIG)
}

func TestConfigSanityCheck(t *testing.T) {
	if err := CONFIG.SanityCheck(); err != nil {
		t.Fatal(err.Error())
	}
}

func TestConfigCacheID(t *testing.T) {
	c, err := ConfigCreateFromEnv()

	id, err := c.CacheID()
	if err != nil {
		t.Fatal(err.Error())
	} else {
		t.Log(id)
	}

	id, err = c.CacheIDWithContext(nil)

	context, _ := c.CurrentContext()
	id, err = c.CacheIDWithContext(context)
	if err != nil {
		t.Fatal(err.Error())
	} else {
		t.Log(id)
	}
}

func TestConfigDescription(t *testing.T) {
	d, err := CONFIG.Description()
	if err != nil {
		t.Fatal(err.Error())
	}
	t.Log(d)
}

func TestConfigCurrentContext(t *testing.T) {
	c, _ := CurrentConfig()
	p, err := c.CurrentContext()
	if err != nil {
		t.Fatal(err.Error())
	}
	t.Logf("Current Context: %+v", p)
}

func TestConfigSearchPath(t *testing.T) {
	p, err := CONFIG.SearchPath()
	if err != nil {
		t.Fatal(err.Error())
	}
	t.Log(p)
}

func TestConfigWorkingDir(t *testing.T) {
	p, err := CONFIG.WorkingDir()
	if err != nil {
		t.Fatal(err.Error())
	}
	t.Log(p)
}

func TestConfigNumColorSpaces(t *testing.T) {
	n := CONFIG.NumColorSpaces()
	if n <= 0 {
		t.Fatal("Expected number of colorspaces to be greater than 0")
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
				t.Fatal(err.Error())
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
		t.Fatal("Expected number of colorspaces to be greater than 0")
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
				t.Fatal(err.Error())
			}

			idx, err = c.IndexForColorSpace(name)
			if err != nil {
				t.Fatal(err.Error())
			}

			if idx != i {
				t.Fatalf("Expected %d for colorspace %s, got %d", i, name, idx)
			}
		}
	}
}

func TestConfigAddColorSpace(t *testing.T) {
	c := NewConfig()
	names := []string{"Color1", "Color2", "Color3"}

	var actual, expected int

	for i, name := range names {
		cs := NewColorSpace()
		cs.SetName(name)
		c.AddColorSpace(cs)
		expected = i + 1
		actual = c.NumColorSpaces()
		if actual != expected {
			t.Errorf("Expected number of colorspaces to be %d, got %d", expected, actual)
		}
	}

	c.ClearColorSpaces()
	actual = c.NumColorSpaces()
	if actual != 0 {
		t.Fatalf("Expected number of colorspaces to be 0, got %d", actual)
	}
}

func TestConfigParseColorSpace(t *testing.T) {
	var (
		actual     string
		expected   string
		fullString string
		err        error
	)

	tests := map[string]string{
		"linear":   `A bunch of text containing a linear colorspace name`,
		"sRGB":     `A bunch of text containing an srgb colorspace name`,
		"Gamma2.2": `A bunch of text containing both linear and sRGB and gamma2.2 colorspaces`,
	}

	for expected, fullString = range tests {
		actual, err = CONFIG.ParseColorSpaceFromString(fullString)
		if err != nil {
			t.Error(err.Error())
			continue
		}

		if actual != expected {
			t.Errorf("Expected to parse %q from string, but got %q", expected, actual)
		}
	}
}

func TestConfigStrictParsingEnabled(t *testing.T) {
	c := CONFIG
	orig := c.IsStrictParsingEnabled()

	err := c.SetStrictParsingEnabled(!orig)
	if err != nil {
		t.Fatal(err.Error())
	}

	if c.IsStrictParsingEnabled() == orig {
		t.Fatalf("Expected %v, got %v", !orig, orig)
	}

	err = c.SetStrictParsingEnabled(orig)
	if err != nil {
		t.Fatal(err.Error())
	}

	if c.IsStrictParsingEnabled() != orig {
		t.Fatalf("Expected %v, got %v", orig, !orig)
	}
}

func TestRoles(t *testing.T) {
	c := CONFIG

	origCount := c.NumRoles()
	if origCount <= 0 {
		t.Fatal("Expected number of roles to be greater than 0")
	}

	role := `__unittest_role__`

	space, err := c.ColorSpaceNameByIndex(0)
	if err != nil {
		t.Fatal(err.Error())
	}

	err = c.SetRole(role, space)
	if err != nil {
		t.Fatal(err.Error())
	}

	if count := c.NumRoles(); count != (origCount + 1) {
		t.Fatalf("Expected number of roles to be %d, but got %d", origCount+1, count)
	}

	if !c.HasRole(role) {
		t.Fatalf("Expected config to have the role %v", role)
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
		t.Fatalf("Expected to find role name %v in list of roles", role)
	}

	err = c.SetRole(role, "")
	if err != nil {
		t.Fatal(err.Error())
	}
	if count := c.NumRoles(); count != origCount {
		t.Fatalf("Expected number of roles to be %d, but got %d", origCount, count)
	}

	if c.HasRole(role) {
		t.Fatalf("Expected config to not have the role %v", role)
	}
}

func TestConfigProcessor(t *testing.T) {
	cfg, _ := CurrentConfig()
	ct, err := cfg.CurrentContext()
	if err != nil {
		t.Fatal(err.Error())
	}

	proc, err := cfg.Processor(ct, "linear", "sRGB")
	if err != nil {
		t.Fatal("Error getting a Processor with current context, and 'linear', 'sRGB'")
	}

	t.Logf("Processor.IsNoOp: %v", proc.IsNoOp())
	t.Logf("Processor.HasChannelCrosstalk: %v", proc.HasChannelCrosstalk())

	_, err = cfg.Processor("linear", "sRGB")
	if err != nil {
		t.Fatal("Error getting a Processor with 'linear', 'sRGB'")
	}

	_, err = cfg.Processor(ROLE_COMPOSITING_LOG, ROLE_SCENE_LINEAR)
	if err != nil {
		t.Fatal("Error getting a Processor with constants ROLE_COMPOSITING_LOG, ROLE_SCENE_LINEAR")
	}

	_, err = cfg.Processor(ct, ROLE_COMPOSITING_LOG, ROLE_SCENE_LINEAR)
	if err != nil {
		t.Fatal("Error getting a Processor with current context and constants ROLE_COMPOSITING_LOG, ROLE_SCENE_LINEAR")
	}

	// TODO:
	// Debug SIGABRT when using a Processor from a Config created from a data stream
	//
	// ct = NewContext()
	// proc, err = CONFIG.Processor(ct, "linear", "sRGB")
	// if err != nil {
	//     t.Fatal("Error getting a Processor with nil context, and 'linear', 'sRGB'")
	// }
}

func TestConfigDisplaysViews(t *testing.T) {
	var (
		str string
		i   int
		err error
	)

	str = CONFIG.DefaultDisplay()
	if str != "sRGB" {
		t.Errorf("expected DefaultDisplay to be 'sRGB', but got %q", str)
	}

	i = CONFIG.NumDisplays()
	if i != 2 {
		t.Errorf("expected NumDisplays to be 2. but got %d", i)
	}

	str = CONFIG.Display(0)
	if str != "DCIP3" {
		t.Errorf("expected Display at index 0 to be 'DCIP3', but got %q", str)
	}

	str = CONFIG.DefaultView("sRGB")
	if str != "Film" {
		t.Errorf("expected DefaultView for 'sRGB' to be 'Film', but got %q", str)
	}

	if i = CONFIG.NumViews("sRGB"); i != 4 {
		t.Errorf("expected NumViews to be 4. but got %d", i)
	}

	if str = CONFIG.View("sRGB", 2); str != "Raw" {
		t.Errorf("expected View at index 2 to be 'Raw', but got %q", str)
	}

	if str = CONFIG.ActiveDisplays(); str != "sRGB, DCIP3" {
		t.Errorf("expected ActiveDisplays to be 'sRGB, DCIP3', but got %q", str)
	}

	if str = CONFIG.ActiveViews(); str != "Film, Log, Raw" {
		t.Errorf("expected ActiveViews to be 'Film, Log, Raw', but got %q", str)
	}

	if str = CONFIG.DisplayLooks("sRGB", "Film DI"); str != "di" {
		t.Errorf("expected DisplayLooks for 'sRGB' / 'Film DI' to be 'di', but got %q", str)
	}

	if str = CONFIG.DisplayColorSpaceName("sRGB", "Raw"); str != "nc10" {
		t.Errorf("expected DisplayColorSpaceName for 'sRGB' / 'Raw' to be 'nc10', but got %q", str)
	}

	prev := CONFIG.NumViews("sRGB")
	if err = CONFIG.AddDisplay("sRGB", "TEST_VIEW", "vd8", "di"); err != nil {
		t.Error(err.Error())
	}
	if i = CONFIG.NumViews("sRGB"); i != (prev + 1) {
		t.Errorf("expected NumViews for 'sRGB' to be %d, but got %d", prev+1, i)
	}
	if str = CONFIG.DisplayLooks("sRGB", "TEST_VIEW"); str != "di" {
		t.Errorf("expected DisplayLooks for 'sRGB' / 'TEST_VIEW' to be 'di', but got %q", str)
	}

	if err = CONFIG.SetActiveDisplays("DCIP3"); err != nil {
		t.Error(err.Error())
	}
	if str = CONFIG.ActiveDisplays(); str != "DCIP3" {
		t.Errorf("expected ActiveDisplays to be 'DCIP3', but got %q", str)
	}

	if err = CONFIG.SetActiveViews("Log, Raw"); err != nil {
		t.Error(err.Error())
	}
	if str = CONFIG.ActiveViews(); str != "Log, Raw" {
		t.Errorf("expected ActiveViews to be 'Log, Raw', but got %q", str)
	}
}

/*

ColorSpaces

*/
func TestColorSpace(t *testing.T) {
	c := CONFIG

	name, _ := c.ColorSpaceNameByIndex(0)
	cs, err := c.ColorSpace(name)
	if err != nil {
		t.Fatalf("Error getting a ColorSpace from name %s: %s", name, err.Error())
	}
	t.Logf("ColorSpace: %+v", cs)
}

func TestColorSpaceCreate(t *testing.T) {
	cs := NewColorSpace()
	t.Log(cs)
}

func TestColorSpaceEditableCopy(t *testing.T) {
	cs, _ := CONFIG.ColorSpace("linear")
	cs_copy := cs.EditableCopy()
	t.Logf("%s is a copy of %s", cs_copy, cs)

	if cs.Name() != cs_copy.Name() {
		t.Fatalf("Copy colorspace name is %s, but expected %s", cs_copy.Name(), cs.Name())
	}
}

func TestColorSpaceName(t *testing.T) {
	cs, _ := CONFIG.ColorSpace("linear")
	cs.SetName("FOO")
	defer cs.SetName("linear")

	if cs.Name() != "FOO" {
		t.Fatalf("Expected ColorSpace name to be FOO, got %s", cs.Name())
	}
}

func TestColorSpaceFamily(t *testing.T) {
	cs, _ := CONFIG.ColorSpace("linear")
	family := cs.Family()
	cs.SetFamily("FOO")
	defer cs.SetFamily(family)

	if cs.Family() != "FOO" {
		t.Fatalf("Expected ColorSpace family to be FOO, got %s", cs.Family())
	}
}

func TestColorSpaceEqualityGroup(t *testing.T) {
	cs, _ := CONFIG.ColorSpace("linear")
	group := cs.EqualityGroup()
	cs.SetEqualityGroup("FOO")
	defer cs.SetEqualityGroup(group)

	if cs.EqualityGroup() != "FOO" {
		t.Fatalf("Expected ColorSpace EqualityGroup to be FOO, got %s", cs.EqualityGroup())
	}
}

func TestColorSpaceDescription(t *testing.T) {
	cs, err := CONFIG.ColorSpace("linear")
	if err != nil {
		t.Fatal(err.Error())
	}
	desc := cs.Description()
	cs.SetDescription("FOO")
	defer cs.SetDescription(desc)

	if cs.Description() != "FOO" {
		t.Fatalf("Expected ColorSpace Description to be FOO, got %s", cs.Description())
	}
}

func TestColorSpaceBitDepth(t *testing.T) {
	cs, err := CONFIG.ColorSpace("linear")
	if err != nil {
		t.Fatal(err.Error())
	}

	depth := cs.BitDepth()
	defer cs.SetBitDepth(depth)

	depths := []BitDepth{
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
			t.Fatalf("Expected ColorSpace BitDepth to be %v, got %v", d, cs.BitDepth())
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
		t.Fatalf("Expected FOO=BAR, got %s", c_copy.StringVar("FOO"))
	}

}

func TestContextCacheID(t *testing.T) {
	c, _ := CurrentConfig()
	context, _ := c.CurrentContext()

	id, err := context.CacheID()
	if err != nil {
		t.Fatal(err.Error())
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
		t.Fatalf("Expected search path to be %q, got %q", expected, actual)
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
		t.Fatalf("Expected working dir to be %q, got %q", expected, actual)
	}
}

func TestContextLoadEnvironment(t *testing.T) {
	c := NewContext()
	c.LoadEnvironment()
}

/*

ImageDesc

*/

func TestPackedImageDesc(t *testing.T) {
	width, height, channels := 128, 64, 3
	imgDesc, imageData := getGradImageDesc(width, height, channels)

	if fmt.Sprintf("%v", imageData) != fmt.Sprintf("%v", imgDesc.Data()) {
		t.Error("Original RGB data is not equal to PackedImageDesc.Data()")
	}

	if imgDesc.Width() != width {
		t.Errorf("expected width %d, but got %d", width, imgDesc.Width())
	}

	if imgDesc.Height() != height {
		t.Errorf("expected height %d, but got %d", height, imgDesc.Height())
	}

	if imgDesc.NumChannels() != channels {
		t.Errorf("expected channels %d, but got %d", channels, imgDesc.NumChannels())
	}
}

func TestProcessorApply(t *testing.T) {
	width, height, channels := 512, 256, 3
	imgDesc, imageData := getGradImageDesc(width, height, channels)

	imageDataCopy := make(ColorData, len(imageData))
	copy(imageDataCopy, imageData)

	cfg, _ := CurrentConfig()
	ct, _ := cfg.CurrentContext()
	processor, _ := cfg.Processor(ct, "linear", "Cineon")

	processor.Apply(imgDesc)

	if fmt.Sprintf("%v", imageDataCopy) == fmt.Sprintf("%v", imgDesc.Data()) {
		t.Fatal("Original RGB data remained unchanged after Apply()")
	}
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

func getImageData(width, height, channels int) ColorData {
	var (
		pix   int
		color float32
	)

	imageData := make(ColorData, width*height*channels)

	for row := 0; row < height; row++ {
		color = float32(row) / float32(height)
		for col := 0; col < width; col++ {
			imageData[pix] = color
			imageData[pix+1] = color
			imageData[pix+2] = color

			if channels == 4 {
				imageData[pix+3] = 1.0
			}

			pix += channels
		}
	}

	return imageData
}

func getGradImageDesc(width, height, channels int) (*PackedImageDesc, ColorData) {
	imageData := getImageData(width, height, channels)
	return NewPackedImageDesc(imageData, width, height, channels), imageData
}

const OCIO_CONFIG = `
ocio_profile_version: 1

search_path: luts
strictparsing: true
luma: [0.2126, 0.7152, 0.0722]

roles:
  color_picking: cpf
  color_timing: lg10
  compositing_log: lgf
  data: ncf
  default: ncf
  matte_paint: vd8
  reference: lnf
  scene_linear: lnf
  texture_paint: dt16

displays:
  DCIP3:
    - !<View> {name: Film, colorspace: p3dci8}
    - !<View> {name: Log, colorspace: lg10}
    - !<View> {name: Raw, colorspace: nc10}
    - !<View> {name: Film DI, colorspace: p3dci8, looks: di}
  sRGB:
    - !<View> {name: Film, colorspace: srgb8}
    - !<View> {name: Log, colorspace: lg10}
    - !<View> {name: Raw, colorspace: nc10}
    - !<View> {name: Film DI, colorspace: srgb8, looks: di}

active_displays: [sRGB, DCIP3]
active_views: [Film, Log, Raw]

colorspaces:
  - !<ColorSpace>
    name: lnf
    family: ln
    equalitygroup: 
    bitdepth: 32f
    description: |
      lnf :  linear show space
    isdata: false
    allocation: lg2
    allocationvars: [-15, 6]

  - !<ColorSpace>
    name: lnh
    family: ln
    equalitygroup: 
    bitdepth: 16f
    description: |
      lnh :  linear show space
    isdata: false
    allocation: lg2
    allocationvars: [-15, 6]

  - !<ColorSpace>
    name: ln16
    family: ln
    equalitygroup: 
    bitdepth: 16ui
    description: |
      ln16 : linear show space
    isdata: false
    allocation: lg2
    allocationvars: [-15, 0]

  - !<ColorSpace>
    name: lg16
    family: lg
    equalitygroup: 
    bitdepth: 16ui
    description: |
      lg16 : conversion from film log 
    isdata: false
    allocation: uniform
    to_reference: !<FileTransform> {src: lg16.spi1d, interpolation: nearest}

  - !<ColorSpace>
    name: lg10
    family: lg
    equalitygroup: 
    bitdepth: 10ui
    description: |
      lg10 : conversion from film log
    isdata: false
    allocation: uniform
    to_reference: !<FileTransform> {src: lg10.spi1d, interpolation: nearest}

  - !<ColorSpace>
    name: lgf
    family: lg
    equalitygroup: 
    bitdepth: 32f
    description: |
      lgf : conversion from film log
    isdata: false
    allocation: uniform
    allocationvars: [-0.25, 1.5]
    to_reference: !<FileTransform> {src: lgf.spi1d, interpolation: linear}

  - !<ColorSpace>
    name: gn10
    family: gn
    equalitygroup: 
    bitdepth: 10ui
    description: |
      gn10 :conversion from Panalog
    isdata: false
    allocation: uniform
    to_reference: !<FileTransform> {src: gn10.spi1d, interpolation: nearest}

  - !<ColorSpace>
    name: vd16
    family: vd
    equalitygroup: 
    bitdepth: 16ui
    description: |
      vd16 :conversion from a gamma 2.2 
    isdata: false
    allocation: uniform
    to_reference: !<GroupTransform>
      children:
        - !<FileTransform> {src: version_8_whitebalanced.spimtx, interpolation: unknown, direction: inverse}
        - !<FileTransform> {src: vd16.spi1d, interpolation: nearest}

  - !<ColorSpace>
    name: vd10
    family: vd
    equalitygroup: 
    bitdepth: 10ui
    description: |
      vd10 :conversion from a gamma 2.2 
    isdata: false
    allocation: uniform
    to_reference: !<GroupTransform>
      children:
        - !<FileTransform> {src: version_8_whitebalanced.spimtx, interpolation: unknown, direction: inverse}
        - !<FileTransform> {src: vd10.spi1d, interpolation: nearest}

  - !<ColorSpace>
    name: vd8
    family: vd
    equalitygroup: 
    bitdepth: 8ui
    description: |
      vd8 :conversion from a gamma 2.2
    isdata: false
    allocation: uniform
    to_reference: !<GroupTransform>
      children:
        - !<FileTransform> {src: version_8_whitebalanced.spimtx, interpolation: unknown, direction: inverse}
        - !<FileTransform> {src: vd8.spi1d, interpolation: nearest}

  - !<ColorSpace>
    name: hd10
    family: hd
    equalitygroup: 
    bitdepth: 10ui
    description: |
      hd10 : conversion from REC709
    isdata: false
    allocation: uniform
    to_reference: !<GroupTransform>
      children:
        - !<FileTransform> {src: hdOffset.spimtx, interpolation: nearest, direction: inverse}
        - !<ColorSpaceTransform> {src: vd16, dst: lnf}

  - !<ColorSpace>
    name: dt16
    family: dt
    equalitygroup: 
    bitdepth: 16ui
    description: |
      dt16 :conversion for diffuse texture
    isdata: false
    allocation: uniform
    to_reference: !<GroupTransform>
      children:
        - !<FileTransform> {src: diffuseTextureMultiplier.spimtx, interpolation: nearest}
        - !<ColorSpaceTransform> {src: vd16, dst: lnf}

  - !<ColorSpace>
    name: cpf
    family: cp
    equalitygroup: 
    bitdepth: 32f
    description: |
      cpf :video like conversion used for color picking 
    isdata: false
    allocation: uniform
    to_reference: !<FileTransform> {src: cpf.spi1d, interpolation: linear}

  - !<ColorSpace>
    name: nc8
    family: nc
    equalitygroup: 
    bitdepth: 8ui
    description: |
      nc8 :nc,Non-color used to store non-color data such as depth or surface normals
    isdata: true
    allocation: uniform

  - !<ColorSpace>
    name: nc10
    family: nc
    equalitygroup: 
    bitdepth: 10ui
    description: |
      nc10 :nc,Non-color used to store non-color data such as depth or surface normals
    isdata: true
    allocation: uniform

  - !<ColorSpace>
    name: nc16
    family: nc
    equalitygroup: 
    bitdepth: 16ui
    description: |
      nc16 :nc,Non-color used to store non-color data such as depth or surface normals
    isdata: true
    allocation: uniform

  - !<ColorSpace>
    name: ncf
    family: nc
    equalitygroup: 
    bitdepth: 32f
    description: |
      ncf :nc,Non-color used to store non-color data such as depth or surface normals
    isdata: true
    allocation: uniform

  - !<ColorSpace>
    name: srgb8
    family: srgb
    equalitygroup: 
    bitdepth: 8ui
    description: |
      srgb8 :rgb display space for the srgb standard.
    isdata: false
    allocation: uniform
    from_reference: !<GroupTransform>
      children:
        - !<ColorSpaceTransform> {src: lnf, dst: lg10}
        - !<FileTransform> {src: spi_ocio_srgb_test.spi3d, interpolation: linear}

  - !<ColorSpace>
    name: p3dci8
    family: p3dci
    equalitygroup: 
    bitdepth: 8ui
    description: |
      p3dci8 :rgb display space for gamma 2.6 P3 projection.
    isdata: false
    allocation: uniform
    from_reference: !<GroupTransform>
      children:
        - !<ColorSpaceTransform> {src: lnf, dst: lg10}
        - !<FileTransform> {src: colorworks_filmlg_to_p3.3dl, interpolation: linear}

  - !<ColorSpace>
    name: xyz16
    family: xyz
    equalitygroup: 
    bitdepth: 16ui
    description: |
      xyz16 :Conversion for  DCP creation.
    isdata: false
    allocation: uniform
    from_reference: !<GroupTransform>
      children:
        - !<ColorSpaceTransform> {src: lnf, dst: p3dci8}
        - !<ExponentTransform> {value: [2.6, 2.6, 2.6, 1]}
        - !<FileTransform> {src: p3_to_xyz16_corrected_wp.spimtx, interpolation: unknown}
        - !<ExponentTransform> {value: [2.6, 2.6, 2.6, 1], direction: inverse}

looks:
- !<Look>
  name: di
  process_space: p3dci8
  transform: !<FileTransform> {src: look_di.cc, interpolation: linear}
`
