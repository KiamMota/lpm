#!/bin/bash

if command -v lpm &>/dev/null; then
  echo "old lpm found, removing..."
  sudo rm -f $(which lpm)
fi

go build .
go install ./cmd/lpm

if ! echo $PATH | grep -q "$HOME/go/bin"; then
  echo 'export PATH=$HOME/go/bin:$PATH' >>~/.zshrc
  export PATH=$HOME/go/bin:$PATH
fi

echo "ok."
