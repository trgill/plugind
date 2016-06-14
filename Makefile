
BUILDDIRS = \
        src \
        plugins 

all:
	@for dir in $(BUILDDIRS); do \
	$(MAKE) -C $$dir || exit $?; \
	done

clean:
	@for dir in $(BUILDDIRS); do \
	$(MAKE) -C $$dir clean || exit $?; \
	done



