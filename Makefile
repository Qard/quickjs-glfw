# TODO:
# - make shared library build optional
# - figure out single-binary build with `qjsc -M module.so,module ...`

CFLAGS=-fPIC -DJS_SHARED_LIBRARY -I./
LIBS=-lquickjs -lglfw
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))

.PHONY: build
build: glfw.so

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

glfw.so: $(OBJS)
	$(CC) -shared -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -f *.so *.o
