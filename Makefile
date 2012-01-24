#
# Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
#

.PHONY: default xcode test

default:
	$(MAKE) -C Native default

xcode:
	$(MAKE) -C Native xcode

test:
	$(MAKE) -C Native test
