package cli

func Help() {
	print(`

lpm — lazy plugin manager

usage:  lpm <command> [argument]

commands:
  install    <github url>   download and create plugin config
  remove     <plugin>       delete plugin config file
  able       <plugin>       toggle enabled in plugin config
  cat        <plugin>       print plugin config
  list                      list installed plugins
  sync                      run :Lazy sync headless

flags:
  -v, --version             show version
	-f, --fetch               show infos about lpm 
  -u, --self-update         update lpm
  -h, --help                show this message

examples:
  lpm install https://github.com/nvim-telescope/telescope.nvim
  lpm able telescope
  lpm cat telescope
  lpm sync
`)
}
