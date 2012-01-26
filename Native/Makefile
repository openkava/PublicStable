#
# Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
#

.PHONY: default xcode test

default:
	scons -kj8

xcode:
	PYTHONUNBUFFERED=1 scons

test: build
	$(MAKE) -C Test
