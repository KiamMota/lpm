package base

import (
	"bufio"
	"fmt"
	"os"
	"path/filepath"
	"strconv"
	"strings"
)

func WhenMultiplePluginsFound(arr []string) string {
	if len(arr) == 0 {
		LpmLog("plugin not found!", Err)
		os.Exit(1)
	}
	if len(arr) == 1 {
		return filepath.Join(NvPath.PluginsPath, arr[0]+".lua")
	}
	LpmLog("multiple plugins found:", Warn)
	for i, plugin := range arr {
		fmt.Printf("  [%d] %s\n", i+1, plugin)
	}
	fmt.Println()
	reader := bufio.NewReader(os.Stdin)
	for {
		fmt.Printf("Select a plugin (1-%d): ", len(arr))
		input, err := reader.ReadString('\n')
		if err != nil {
			fmt.Fprintln(os.Stderr, "error reading input")
			os.Exit(1)
		}
		input = strings.TrimSpace(input)
		n, err := strconv.Atoi(input)
		if err != nil || n < 1 || n > len(arr) {
			fmt.Printf("  invalid choice, enter a number between 1 and %d\n", len(arr))
			continue
		}
		return filepath.Join(NvPath.PluginsPath, arr[n-1]+".lua")
	}
}
