cc=g++
bin=http_server

.PHONY:all
all:$(bin)

$(bin):src/*.cpp
	$(cc) $^ -o $@ -std=c++11 -lpthread

.PHONY:clean
clean:
	-rm -rf $(bin)
