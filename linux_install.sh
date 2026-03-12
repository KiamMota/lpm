#!/bin/bash

set -e

VERSION="1.0.0"
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

URL="https://github.com/KiamMota/lpm/releases/download/v${VERSION}/lpm_${VERSION}_${OS}_${ARCH}.tar.gz"

echo "==> downloading lpm ${VERSION} (${OS}/${ARCH})..."
curl -fsSL "$URL" -o /tmp/lpm.tar.gz

echo "==> extracting..."
tar -xzf /tmp/lpm.tar.gz -C /tmp

if command -v lpm &>/dev/null; then
  echo "==> removing old lpm..."
  sudo rm -f "$(which lpm)"
fi

echo "==> installing..."
sudo mv /tmp/lpm /usr/local/bin/lpm
sudo chmod +x /usr/local/bin/lpm

rm -f /tmp/lpm.tar.gz

echo "==> lpm ${VERSION} installed!"
lpm --version
