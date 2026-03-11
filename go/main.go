package main

import (
	"lpm/cli"
	"lpm/commands"
	"os"
)

func main() {
	if len(os.Args) == 1 {
		cli.Help()
		return
	}

	var cmd string = os.Args[1]
	if cmd == "-v" || cmd == "--version" {
		cli.Version()
		return
	}

	if cmd == "list" {
		commands.List()
		return
	}

}
