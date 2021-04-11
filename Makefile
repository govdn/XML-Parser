all:	parser.c Test.c
	gcc -o parser Test.c parser.c