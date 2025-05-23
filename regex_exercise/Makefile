SRCS := main
OBJS = $(SRCS:%=objs/%.o)
DEPENDS = $(SRCS:%=objs/%.d)
CC = c++
FLAGS = -MD -g -std=c++17 \
	-Iftxui_install/include
LDFLAGS = -Lftxui_install/lib \
	-lftxui-component -lftxui-screen -lftxui-dom -lncurses -lpthread
NAME = Regex

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

objs/%.o: srcs/%.cpp Makefile | objs/
	$(CC) -c $< -o $@ $(FLAGS)

objs/: 
	mkdir -p objs

.PHONY: all clean fclean re

clean: 
	rm -f $(OBJS) $(DEPENDS)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPENDS)