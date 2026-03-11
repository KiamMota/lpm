package commands

import (
	"lpm/base"
	"os"
	"path/filepath"
)

func Remove(pluginName string) {
	entries, err := os.ReadDir(base.NvPath.PluginsPath)
	if err != nil {
		base.LpmLog(err.Error(), base.Err)
	}

	for _, entry := range entries {
		if entry.Name() == pluginName {
			var absPluginPath string = filepath.Join(base.NvPath.PluginsPath, entry.Name())
			os.Remove(absPluginPath)
			base.LpmLog("removed"+absPluginPath, base.Ok)
			break
		}
	}
	base.LazySync()

}
