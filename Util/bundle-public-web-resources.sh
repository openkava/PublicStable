#!/bin/sh

DATE=$(date '+%Y-%m-%d')

zip -qr public-web-resources-$DATE.zip \
  Web/Apps/Sample/BasicDemos/Models \
  Web/Apps/Sample/BasicDemos/Resources
