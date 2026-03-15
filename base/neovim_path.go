package base

import (
	"os"
	"path/filepath"
)

type NeovimPath struct {
	BasePath    string
	LuaPath     string
	InitLuaPath string
	PluginsPath string
}

func StartNeovimPath() NeovimPath {
	confDir, _ := os.UserConfigDir()
	var basePath string = filepath.Join(confDir, "nvim")
	nvPath := NeovimPath{
		BasePath:    basePath,
		LuaPath:     filepath.Join(basePath, "lua"),
		InitLuaPath: filepath.Join(basePath, "init.lua"),
		PluginsPath: filepath.Join(basePath, "lua", "plugins"),
	}
	return nvPath
}

var NvPath NeovimPath = StartNeovimPath()
