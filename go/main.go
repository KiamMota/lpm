package main

import (
	"lpm/base"
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
	if len(os.Args) == 2 {
		base.LpmLog("not enough args!", base.Err)
		return
	}
	var args []string = os.Args[2:]

	if cmd == "remove" {
		commands.Remove(args[0])
		return
	}

}
