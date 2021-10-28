ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

xp_pen_hack: xp_pen_hack.cpp
	g++ --std=c++20 -o xp_pen_hack xp_pen_hack.cpp

install: xp_pen_hack
	install xp_pen_hack $(PREFIX)/bin
