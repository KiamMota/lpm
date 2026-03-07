**lazy.nvim-cli** is a tool designed to manage Neovim plugins **without opening the editor**. It acts as an automation layer on top of the Lazy.nvim plugin, allowing you to install, remove, and list plugins directly from the terminal.

## How it works

1. **Installing plugins**

   * You provide the GitHub repository URL of the plugin:

     ```bash
     lazynvimcli install https://github.com/nvim-telescope/telescope.nvim
     ```
   * The CLI automatically creates a `.lua` file in the plugins folder (usually `~/.config/nvim/lua/plugins/`) containing:

     ```lua
     return {
       'https://github.com/nvim-telescope/telescope.nvim',
       dependencies = {},
       opts = {},
       cmd = {},
       keys = {},
     }
     ```
   * This file is ready for Lazy.nvim to manage installation and plugin loading the next time you open Neovim or run `:Lazy sync`.

2. **Listing plugins**

   * You can list all installed plugins with:

     ```bash
     lazynvimcli list
     ```
   * This command shows the `.lua` files in the plugins folder, giving you an overview of installed plugins.

3. **Removing plugins**

   * To remove a plugin, use:

     ```bash
     lazynvimcli remove telescope.nvim.lua
     ```
   * The CLI deletes the corresponding `.lua` file, and Lazy.nvim will uninstall the plugin on the next sync.

4. **Modularity and safety**

   * The CLI avoids modifying your main `init.lua` or config files directly.
   * It generates separate files for each plugin, making maintenance modular and preventing indentation errors or configuration conflicts.

5. **Integration with Lazy.nvim**

   * The files created by the CLI are fully compatible with Lazy.nvim.
   * The CLI can optionally prompt to insert the plugin reference into your `init.lua` automatically.

### Summary

lazy.nvim-cli automates the convenience of Lazy.nvim: you manage plugins without opening Neovim, keep everything modular, avoid manual mistakes, and ensure compatibility when Lazy.nvim syncs in the editor.
