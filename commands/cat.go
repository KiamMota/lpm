package commands

import (
	"fmt"
	"lpm/base"
	"os"
)

func Cat(pluginName string) {
	pluginsFound := base.FuzzyFindPlugin(pluginName)
	selected := base.WhenMultiplePluginsFound(pluginsFound)

	base.LpmLog("content of '"+selected+"':", base.Ok)
	content, err := os.ReadFile(selected)
	if len(content) == 0 {
		base.LpmLog("empty file.", base.Err)
	}
	if err != nil {
		base.LpmLog("error reading file:"+err.Error(), base.Err)
		return
	}

	fmt.Print(string(content))
}
