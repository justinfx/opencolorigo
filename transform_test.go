package ocio

import (
	"testing"
)

func TestDisplayTransform(t *testing.T) {
	dt := NewDisplayTransform()
	// assert interface
	var _ Transform = dt

	if val := dt.Display(); val != "" {
		t.Errorf("expected empty string; got %q", val)
	}
	if val := dt.View(); val != "" {
		t.Errorf("expected empty string; got %q", val)
	}
	if val := dt.InputColorSpace(); val != "" {
		t.Errorf("expected empty string; got %q", val)
	}
	if val := dt.LooksOverride(); val != "" {
		t.Errorf("expected empty string; got %q", val)
	}
	if val := dt.LooksOverrideEnabled(); val {
		t.Errorf("expected false; got %v", val)
	}
	if val := dt.Direction(); val != TRANSFORM_DIR_FORWARD {
		t.Errorf("expected TRANSFORM_DIR_FORWARD(%v); got %v", TRANSFORM_DIR_FORWARD, val)
	}

	dt.SetDisplay("display")
	dt.SetView("view")
	dt.SetInputColorSpace("cs")
	dt.SetLooksOverride("looks")
	dt.SetLooksOverrideEnabled(true)
	dt.SetDirection(TRANSFORM_DIR_INVERSE)

	if val := dt.Display(); val != "display" {
		t.Errorf("expected 'display'; got %q", val)
	}
	if val := dt.View(); val != "view" {
		t.Errorf("expected 'view'; got %q", val)
	}
	if val := dt.InputColorSpace(); val != "cs" {
		t.Errorf("expected 'cs'; got %q", val)
	}
	if val := dt.LooksOverride(); val != "looks" {
		t.Errorf("expected 'looks'; got %q", val)
	}
	if val := dt.LooksOverrideEnabled(); !val {
		t.Errorf("expected true; got %v", val)
	}
	if val := dt.Direction(); val != TRANSFORM_DIR_INVERSE {
		t.Errorf("expected TRANSFORM_DIR_INVERSE(%v); got %v", TRANSFORM_DIR_INVERSE, val)
	}

	cpy := dt.EditableCopy()
	cpy.SetDisplay("display2")
	cpy.SetView("view2")

	if val := cpy.Display(); val != "display2" {
		t.Errorf("expected 'display2'; got %q", val)
	}
	if val := cpy.View(); val != "view2" {
		t.Errorf("expected 'view2'; got %q", val)
	}

	if val := dt.Display(); val != "display" {
		t.Errorf("expected 'display'; got %q", val)
	}
	if val := dt.View(); val != "view" {
		t.Errorf("expected 'view'; got %q", val)
	}
	dt.Destroy()
	cpy.Destroy()
}
