return {
  'williamboman/mason-lspconfig.nvim',
  dependencies = {
    'williamboman/mason.nvim',
    'neovim/nvim-lspconfig',
  },
  config = function()
    require('mason').setup()
    require('mason-lspconfig').setup({
      -- instala automaticamente o LSP da linguagem que você abrir
      automatic_installation = true,
      ensure_installed = {
        'clangd',    -- C/C++
        'lua_ls',    -- Lua
        'pyright',   -- Python
      },
    })
    -- configura automaticamente todo servidor instalado
    require('mason-lspconfig').setup_handlers({
      function(server_name)
        require('lspconfig')[server_name].setup({})
      end,
    })
  end,
}
