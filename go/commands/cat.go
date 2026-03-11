package commands

import (
	"lpm/base"
)

func Cat(pluginName string) {
	var pluginsFound []string = base.FuzzyFindPlugin(pluginName)
	var selected string = base.WhenMultiplePluginsFound(pluginsFound)
	println(selected)
}
