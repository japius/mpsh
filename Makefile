CC = gcc
CFLAGS = -Wall -g -std=c11 -D_XOPEN_SOURCE=700
READLINE = -lreadline -lncurses
# LDLIBS = -lm

LEX=flex #-d
YACC=bison -d

EXEC = mpsh

SRCDIR = src
OBJDIR = obj

SRC := $(shell find $(SRCDIR) -name '*.c')
HEADERS = $(wildcard $(SRCDIR)/headers/*.h)
OBJ = $(addprefix $(OBJDIR)/, $(SRC:.c=.o)) 

$(OBJDIR)/%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $< $(READLINE)

shell: parserc $(OBJ) $(EXEC)

yacclex:
	$(YACC) -v src/parser/parser.y 
	$(LEX) src/parser/lexer.l

parserc:
	$(CC) $(CFLAGS) -o lex.yy.o -c lex.yy.c $(READLINE)
	$(CC) $(CFLAGS) -o parser.tab.o -c parser.tab.c $(READLINE)

$(EXEC): $(OBJ) parser.tab.o lex.yy.o
	$(CC) $(CFLAGS) -o $@ $^ $(READLINE)

clean: 
	rm $(OBJ) $(EXEC) lex.yy.o parser.tab.o parser.output

zip:
	zip mpsh.zip README Makefile $(SRC) $(HEADERS) $(OBJDIR)/$(SRCDIR) $(OBJDIR)/$(SRCDIR)/builtins $(OBJDIR)/$(SRCDIR)/lib equipe lex.yy.c parser.tab.c  parser.tab.h

