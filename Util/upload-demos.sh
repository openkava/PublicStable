#!/bin/sh

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <target>"
  echo "  <target> can be demos.fabric-engine.com, shotgon.fabric-engine.com, etc."
  exit 1
fi
TARGET=$1; shift

S3CMD='s3cmd --acl-public'
S3BUCKET="s3://$TARGET"

$S3CMD put index.html $S3BUCKET/ \
  && $S3CMD sync --delete-removed --exclude '.git/*' Web/ $S3BUCKET/Fabric/
