package main

import (
	"lpm/base"
	"lpm/cli"
	"lpm/commands"
	"os"
)

func main() {

	if !base.NeovimInstalled() {
		base.LpmLog("you dont have nvim in your system!", base.Err)
	}

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

	if cmd == "sync" {
		commands.Sync()
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

	if cmd == "install" {
		commands.Install(args)
	}

	if cmd == "cat" {
		commands.Cat(args[0])
	}

}
