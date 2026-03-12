package cli

import (
	"fmt"
	"lpm/base"
)

func Version() {
	base.LpmLog(base.VersionStr(), base.Ok)
}

func Required(commandName string, arg string) {
	fmt.Println(commandName, "requires", arg)
}
