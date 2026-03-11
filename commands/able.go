package commands

import (
	"lpm/base"
	"os"
	"regexp"
	"strings"
)

func findClosingBrace(content string, openIdx int) int {
	depth := 0
	for i := openIdx; i < len(content); i++ {
		switch content[i] {
		case '{':
			depth++
		case '}':
			depth--
			if depth == 0 {
				return i
			}
		}
	}
	return -1
}

func getTopLevelEnabledValue(block string) ([]int, string) {
	// regex para "enabled = true|false"
	re := regexp.MustCompile(`enabled\s*=\s*(true|false)`)
	matches := re.FindAllStringSubmatchIndex(block, -1)
	if len(matches) == 0 {
		return nil, ""
	}

	// pega o ultimo encontrado
	last := matches[len(matches)-1]

	// last[0] e last[1] sao os indices do match completo enabled = true/false
	// 2 e 3 sao os indices do grupo (true/false)
	value := block[last[2]:last[3]]
	return []int{last[0], last[1]}, value
}

func Able(pluginName string) {
	plugin := base.FuzzyFindPlugin(pluginName)
	selected_plugin := base.WhenMultiplePluginsFound(plugin)
	base.LpmLog("searching for "+selected_plugin, base.Ok)

	bytes, err := os.ReadFile(selected_plugin)
	if err != nil {
		base.LpmLog("cannot open the file!", base.Err)
		return
	}

	fileContent := string(bytes)
	startIdx := strings.Index(fileContent, "return {")
	endIdx := strings.LastIndex(fileContent, "}")
	if startIdx == -1 || endIdx == -1 || startIdx >= endIdx {
		base.LpmLog("invalid Lua table structure", base.Err)
		return
	}

	openBrase := startIdx + len("return ")
	endIdx = findClosingBrace(fileContent, openBrase)

	if endIdx == -1 {
		base.LpmLog("invalid Lua table", base.Err)
		return
	}

	mainBlock := fileContent[startIdx+len("return {") : endIdx]

	loc, value := getTopLevelEnabledValue(mainBlock)

	newValue := "true"
	status := "enabled"
	if value == "true" {
		newValue = "false"
		status = "disabled"
	}

	if loc != nil {
		mainBlock = mainBlock[:loc[0]] + "enabled = " + newValue + mainBlock[loc[1]:]
	} else {
		mainBlock += "\n    enabled = " + newValue
	}

	fileContent = fileContent[:startIdx+len("return {")] + mainBlock + fileContent[endIdx:]

	err = os.WriteFile(selected_plugin, []byte(fileContent), 0644)
	if err != nil {
		base.LpmLog("cannot write the file!", base.Err)
		return
	}

	base.LpmLog("'"+plugin[0]+"' "+status, base.Ok)
}
