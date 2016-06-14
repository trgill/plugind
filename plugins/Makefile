
PLUGINDIRS = \
        multipath \
        lsm 

all:
	@for dir in $(PLUGINDIRS); do \
	$(MAKE) -C $$dir || exit $?; \
	done

clean:
	@for dir in $(PLUGINDIRS); do \
	$(MAKE) -C $$dir clean || exit $?; \
	done
