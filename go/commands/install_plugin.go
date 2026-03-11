package commands

import (
	"io"
	"lpm/base"
	"lpm/cli"
	"net/http"
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

func installRecommendedPluginConfig(pluginName string) (string, error) {
	var url string = "https://raw.githubusercontent.com/KiamMota/lpm/main/plugins_config/" + pluginName + ".lua"
	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		return "", err
	}

	req.Header.Set("User-Agent", "lpm-go/1.0")
	client := &http.Client{}

	resp, err := client.Do(req)
	if err != nil {
		return "", err
	}

	defer resp.Body.Close()

	if resp.StatusCode == 404 {
		return "", nil
	}
	if resp.StatusCode != 200 {
		return "net err", nil
	}

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return "", err
	}

	return string(body), nil

}

func Install(commands []string) {
	if len(commands) == 0 {
		cli.Required("install", "<github url>/<plugin name>")
	}

	var plugin_github_name string = commands[0]
	var plugin_name string = resolvePluginName(plugin_github_name)
	var plugin_path string = base.NvPath.PluginsPath + "/" + plugin_name
	base.LpmLog("resolved plugin name ("+plugin_name+")", base.Ok)
	base.LpmLog("resolved plugin filepath ("+plugin_path+")", base.Ok)
	base.LpmLog("searching recommended plugin config...", base.Ok)

	body, err := installRecommendedPluginConfig(plugin_name)
	if err != nil {
		base.LpmLog("http error!", base.Err)
		return
	}

	if body != "" {
		base.LpmLog("find!", base.Ok)
	}

}
