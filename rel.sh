#!/bin/bash
VERSION=$1
if [ -z "$VERSION" ]; then
  echo "usage: ./rel.sh v1.0.0"
  exit 1
fi

git tag -d $VERSION 2>/dev/null
git push origin --delete $VERSION 2>/dev/null

git add .
git commit -m "release: $VERSION" 2>/dev/null

git tag $VERSION
git push origin main
git push origin $VERSION

echo "✓ release $VERSION pushed!"
