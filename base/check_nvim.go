package base

import (
	"os/exec"
)

func NeovimInstalled() bool {
	_, err := exec.LookPath("nvim")
	return err == nil
}
