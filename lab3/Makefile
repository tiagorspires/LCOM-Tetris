# name of the program (Minix service)
PROG=lab3 

# source code files to be compiled
SRCS = lab3.c keyboard.c utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
<<<<<<< HEAD
CFLAGS += -pedantic
=======
CPPFLAGS += -pedantic -DLAB3
>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
