package base

import "strconv"

const (
	VERSION_MAJOR = 1
	VERSION_MINOR = 0
	VERSION_PATCH = 0
)

func VersionStr() string {
	return strconv.Itoa(VERSION_MAJOR) + "." + strconv.Itoa(VERSION_MINOR) + "." + strconv.Itoa(VERSION_PATCH)
}
