.PHONY: test

test :
	. ../helpers.sh; \
        TMPFILENAME=$$(tmpfilename); \
        ./gen-makefile.sh *.kl >$$TMPFILENAME; \
        $(MAKE) -f $$TMPFILENAME; \
        RESULT=$$?; \
        rm $$TMPFILENAME; \
        exit $$RESULT
