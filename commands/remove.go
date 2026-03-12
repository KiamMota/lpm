package commands

import (
	"lpm/base"
	"os"
	"path/filepath"
)

func Remove(pluginName string) {
	plugin_fuzzy := base.FuzzyFindPlugin(pluginName)
	if len(plugin_fuzzy) == 0 {
		base.LpmLog("plugin named "+pluginName+" not found! (fuzzy tried)", base.Ok)
		return
	}
	plugin_selected := base.WhenMultiplePluginsFound(plugin_fuzzy)
	entries, err := os.ReadDir(base.NvPath.PluginsPath)
	if err != nil {
		base.LpmLog(err.Error(), base.Err)
	}
	for _, entry := range entries {
		if filepath.Join(base.NvPath.PluginsPath, entry.Name()) == plugin_selected {
			err := os.Remove(plugin_selected)
			if err != nil {
				base.LpmLog(err.Error(), base.Err)
				return
			}
			base.LpmLog("removed "+plugin_selected, base.Ok)
			return
		}
	}
	base.LazySync()
}
