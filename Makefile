NAME = ircserv
SRCS = main.cpp Client.cpp Server.cpp ./commands/Nick.cpp \
		./commands/User.cpp \
		./commands/Pass.cpp \
		./commands/Ping.cpp ./commands/Join.cpp ./commands/Privmsg.cpp \
		./commands/Quit.cpp \
		Channel.cpp Parser.cpp \
		./commands/RunCommands.cpp
OBJS = $(SRCS:.cpp=.o)
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
all: $(NAME)
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -f $(OBJS)
fclean: clean
	rm -f $(NAME)
re: fclean all
.PHONY: all clean fclean re