#!/bin/sh

if [ "$#" -ne 1 ]; then
  echo "Usage: upload-dist.sh <version>"
  echo "Use 'latest' to make the version the latest"
  exit 1
fi
VERSION="$1"; shift

upload() {
  filename=$1; shift
  mimeType=$1; shift
  s3cmd put --acl-public --mime-type=$mimeType $filename s3://dist.fabric-engine.com/$VERSION/$fileName
}

for OS_ARCH in Windows-x86 Darwin-i386 Darwin-x86_64 Linux-i686 Linux-x86_64; do
  upload FabricEngine-$OS_ARCH.crx application/x-chrome-extension
  upload crx-update-$OS_ARCH.xml application/xml
  upload FabricEngine-$OS_ARCH.xpi application/x-xpinstall
  upload xpi-update-$OS_ARCH.rdf text/xml
done
