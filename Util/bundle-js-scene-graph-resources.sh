#!/bin/sh

DATE=$(date '+%Y-%m-%d')

zip -qr js-scene-graph-resources-$DATE.zip \
  Apps/Sample/BasicDemos/Models \
  Apps/Sample/BasicDemos/Resources \
  Apps/Sample/BasicDemos/Resources/cubemap_opensea \
  Apps/Sample/Characters/Assets \
  Apps/Sample/Rendering/Resources \
  Apps/Sample/Tutorials/Resources \
  Apps/Sample/Alembic/Models \
  Apps/Sample/Lidar/Models \
  Apps/Sample/UseCases/V8Engine/Models \
  Apps/Sample/UseCases/BulletPlayGround/Resources
