package base

import (
	"os"
)

type NeovimPath struct {
	BasePath    string
	LuaPath     string
	InitLuaPath string
}

func StartNeovimPath() NeovimPath {
	confDir, _ := os.UserConfigDir()
	var basePath string = confDir + "/nvim"
	nvPath := NeovimPath{BasePath: basePath, LuaPath: basePath + "/lua", InitLuaPath: basePath + "/init.lua"}
	return nvPath
}

var nvPath NeovimPath = StartNeovimPath()
