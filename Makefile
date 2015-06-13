DIR_LIST=./Blinker433\
	./LightBell\
	./NeoClock\
	./RGBLedWalker\
	./simple

DO_LIST=@for f in $(DIR_LIST); \
			do pushd sketches/$$f; \
			$(MAKE) $@; \
			popd; \
			done; \
			echo $@ done

all:
	$(DO_LIST)

clean:
	$(DO_LIST)


