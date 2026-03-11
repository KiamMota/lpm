package commands

import (
	"lpm/base"
	"os"
)

func List() {
	entries, err := os.ReadDir(base.NvPath.LuaPath + "/plugins")
	if err != nil {
		base.LpmLog(err.Error(), base.Err)
	}

	base.LpmLog("plugins for '"+base.NvPath.LuaPath+"/plugins"+"':", base.Ok)
	for _, entr := range entries {
		if entr.Type().IsRegular() {
			println("-- ", entr.Name())
		}
	}
}
