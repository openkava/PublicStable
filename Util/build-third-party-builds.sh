#!/bin/sh

if [ $# != 3 ]
then
  echo "Usage: $0 <OS> <ARCH> <TYPE>"
  exit 1
fi

OS=$1; shift
ARCH=$1; shift
TYPE=$1; shift
DATE=$(date '+%Y-%m-%d')

if [ "$OS" = "Windows" ]; then
  ARCHIVE="zip -qr third-party-builds-$OS-$ARCH-$DATE.zip"
else
  ARCHIVE="tar jcf third-party-builds-$OS-$ARCH-$DATE.tar.bz2"
fi
$ARCHIVE Native/ThirdParty/Private/$OS/$ARCH/$TYPE/lib
