
CC=g++
CFLAGS=-ggdb -std=c++11 -Wall -Wextra -pedantic -Werror
DEPS = httpd.h Framer.hpp Parser.hpp
SRCS = httpd.cpp DieWithError.c HandleTCPClient.c Framer.cpp Parser.cpp
MAIN_SRCS = main.c $(SRCS)
MAIN_OBJS = $(MAIN_SRCS:.c=.o)

default: httpd

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

httpd:    $(MAIN_OBJS)
	$(CC) $(CFLAGS) -o httpd $(MAIN_OBJS) -lpthread

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f httpd *.o

# CC=g++
# CFLAGS=-ggdb -Wall -Wextra -pedantic -Werror

# H_FILES=CalcFramer.hpp CalcParser.hpp
# FILES=CalcServer.c DieWithError.c HandleTCPClient.c CalcFramer.cpp CalcParser.cpp

# all: httpd

# httpd: $(FILES) $(H_FILES)
	# $(CC) $(CFLAGS) -o httpd $(FILES) -lpthread

# clean:
	# rm -rf httpd *.o
