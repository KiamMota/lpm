#!/bin/bash
VERSION=$1
if [ -z "$VERSION" ]; then
  echo "usage: ./release.sh v1.0.0"
  exit 1
fi

git add .
git commit -m "release: $VERSION" 2>/dev/null
git tag $VERSION
git push origin main
git push origin $VERSION

echo "release $VERSION pushed! GitHub Actions will build and upload the binaries."
