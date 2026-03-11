package commands

import (
	"fmt"
	"lpm/base"
	"os"
)

func Cat(pluginName string) {
	pluginsFound := base.FuzzyFindPlugin(pluginName)
	selected := base.WhenMultiplePluginsFound(pluginsFound)

	content, err := os.ReadFile(selected)
	if err != nil {
		base.LpmLog("error reading file:"+err.Error(), base.Err)
		return
	}

	fmt.Print(string(content))
}
