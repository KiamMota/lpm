package cli

import (
	"fmt"
	"lpm/base"
)

func Fetch() {
	fmt.Printf(`    
                  Z    
                z
   __         z   
  / /__  __ _    version: %s  
 / / _ \/  ' \   nvim path: %s
/_/ .__/_/_/_/   http client: %s
 /_/             language: go 
          
`, base.VersionStr(), base.NvPath.BasePath, "lpm/1.0")
}
