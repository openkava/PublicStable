#!/bin/sh

S3CMD='s3cmd --acl-public'
S3BUCKET='s3://demos.fabric-engine.com'

$S3CMD put index.html $S3BUCKET/ \
  && $S3CMD sync Web/ $S3BUCKET/Fabric/
