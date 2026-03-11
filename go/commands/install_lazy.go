package commands

import (
	"lpm/base"
	"os"
)

func lazyBootstrap() string {
	return `-- Bootstrap lazy.nvim
local lazypath = vim.fn.stdpath("data") .. "/lazy/lazy.nvim"
if not (vim.uv or vim.loop).fs_stat(lazypath) then
  local lazyrepo = "https://github.com/folke/lazy.nvim.git"
  local out = vim.fn.system({ "git", "clone", "--filter=blob:none", "--branch=stable", lazyrepo, lazypath })
  if vim.v.shell_error ~= 0 then
    vim.api.nvim_echo({
      { "Failed to clone lazy.nvim:\n", "ErrorMsg" },
      { out, "WarningMsg" },
      { "\nPress any key to exit..." },
    }, true, {})
    vim.fn.getchar()
    os.exit(1)
  end
end
vim.opt.rtp:prepend(lazypath)

vim.g.mapleader = " "
vim.g.maplocalleader = "\\"

require("lazy").setup({
  spec = {
    { import = "plugins" },
  },
  install = { colorscheme = { "habamax" } },
  checker = { enabled = true },
})`
}

func InstallLazy() {

	base.LpmLog("lazy installation for lazy plugin manager...", base.Ok)
	if !base.DirExists(base.NvPath.LuaPath + "/config") {
		err := os.Mkdir(base.NvPath.LuaPath+"/config", 0755)

		if err != nil {
			base.LpmLog(err.Error(), base.Err)
			return
		}
	}

	lazyFd, lazyFileErr := os.Create(base.NvPath.LuaPath + "/config/lazy.lua")
	if lazyFileErr != nil {
		base.LpmLog(lazyFileErr.Error(), base.Err)
		return
	}

	lazyFd.WriteString(lazyBootstrap())
	base.LpmLog("done.", base.Ok)
}
