package base

import (
	"os"
)

func FileExists(filename string) bool {
	_, err := os.Stat(filename)
	if err == nil {
		return true
	}
	return false

}
