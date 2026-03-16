#!/usr/bin/env bash

set -e

REPO="KiamMota/lpm"

OS=$(uname -s | tr '[:upper:]' '[:lower:]')
ARCH=$(uname -m)

case "$ARCH" in
x86_64) ARCH="amd64" ;;
aarch64) ARCH="arm64" ;;
armv7l) ARCH="arm" ;;
*)
  echo "error: unsupported architecture: $ARCH"
  exit 1
  ;;
esac

if [ "$OS" != "linux" ] && [ "$OS" != "darwin" ]; then
  echo "error: unsupported OS: $OS"
  exit 1
fi

echo "==> detecting latest release..."

VERSION=$(curl -fsSL https://api.github.com/repos/$REPO/releases/latest | grep '"tag_name"' | sed -E 's/.*"v([^"]+)".*/\1/')

if [ -n "$VERSION" ]; then
  echo "==> latest version: $VERSION"

  URL="https://github.com/$REPO/releases/download/v${VERSION}/lpm_${VERSION}_${OS}_${ARCH}.tar.gz"

  echo "==> downloading..."
  curl -fsSL "$URL" -o /tmp/lpm.tar.gz

  echo "==> extracting..."
  tar -xzf /tmp/lpm.tar.gz -C /tmp

  echo "==> installing..."
  sudo mv /tmp/lpm /usr/local/bin/lpm
  sudo chmod +x /usr/local/bin/lpm

  rm -f /tmp/lpm.tar.gz

else
  echo "==> failed to get release, compiling with go..."

  if ! command -v go &>/dev/null; then
    echo "error: go not installed"
    exit 1
  fi

  go install github.com/$REPO@latest

  sudo mv "$(go env GOPATH)/bin/lpm" /usr/local/bin/lpm
fi

echo "==> installed:"
lpm --version
