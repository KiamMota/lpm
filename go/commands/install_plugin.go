package commands

import (
	"lpm/base"
	"lpm/cli"
	"strings"
)

func resolvePluginName(name string) string {
	result := name

	if i := strings.LastIndex(result, "/"); i != -1 {
		result = result[i+1:]
	}

	if strings.HasSuffix(result, ".git") {
		result = result[:len(result)-4]
	}

	suffixes := []string{
		".nvim", "-nvim",
		".neovim", "-neovim",
		".vim", "-vim",
		".lua", "-lua",
	}

	for _, s := range suffixes {
		if strings.HasSuffix(result, s) {
			result = result[:len(result)-len(s)]
			break
		}
	}

	result = strings.ToLower(result)
	result = strings.ReplaceAll(result, ".", "-")

	return result

}

func Install(commands []string) {
	if len(commands) == 0 {
		cli.Required("install", "<github url><plugin name>")
	}

	var plugin_github_url string = resolvePluginName(commands[0])
	var plugin_path string = base.NvPath.PluginsPath + "/" + plugin_github_url
	base.LpmLog("resolved plugin name ("+plugin_github_url+")", base.Ok)
	base.LpmLog("resolved plugin filepath ("+plugin_path+")", base.Ok)
}
