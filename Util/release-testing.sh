#!/bin/sh

error()
{
  if [ -n "$1" ]; then
    echo "$@"
  fi
  echo "An error has occurred; aborting."
  exit 1
}

rexec()
{
  echo ssh fabric-engine.com "$@"
  ssh fabric-engine.com "$@"
}

VERSION="$1"
[ -n "$VERSION" ] || error "Usage: $0 [-f] <version>"
(echo "$VERSION" | egrep -q '^[0-9]{1,2}\.[0-9]{1,2}\.[0-9]{1,2}$') || error "Version number must be of the form 1.0.19, 2.1.0, etc."
DIST_DIR="/fabric-distribution/$VERSION"

rexec ln -snf "$VERSION" "/fabric-distribution/stable"
rexec ln -snf "$DIST_DIR/bin" "/var/www/dist.stable.fabric-engine.com/$VERSION"
echo "Version $VERSION is now the STABLE version"
