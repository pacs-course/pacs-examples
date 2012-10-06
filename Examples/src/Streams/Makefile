
DIRS := ${shell find * -maxdepth 0 -type d -print}

.phony: all clean doc echo

all:
	-for dir in $(DIRS); do (cd $$dir; $(MAKE) all ); done

clean:
	-for dir in $(DIRS); do (cd $$dir; $(MAKE) clean ); done

doc:
	-for dir in $(DIRS); do (cd $$dir; $(MAKE) doc ); done

echo:
	@echo directory list:
	@echo $(DIRS) | tr ' ' '\n'
