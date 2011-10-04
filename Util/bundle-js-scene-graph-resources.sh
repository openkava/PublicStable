#!/bin/sh

DATE=$(date '+%Y-%m-%d')

zip -qr js-scene-graph-resources-$DATE.zip \
  Apps/Sample/BasicDemos/Models \
  Apps/Sample/BasicDemos/Resources \
  Apps/Sample/BasicDemos/Resources/cubemap_opensea \
  Apps/Sample/Tutorials/Resources \
  Apps/Sample/Alembic/Models \
  Apps/Sample/UseCases/V8Engine/Models
