package base

import (
	"os/exec"
)

func LazySync() {
	LpmLog("syncing...", Ok)
	cmd := exec.Command("nvim", "--headless", "-c", "LazySync", "-c", "qa!")
	cmd.Run()
	LpmLog("sync!", Ok)
}
