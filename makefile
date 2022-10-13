CFLAGS = -mfpmath=sse -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -std=gnu99 -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wold-style-definition -Wdeclaration-after-statement -Wbad-function-cast -Wnested-externs -g
all: TestGenerator clear
TestGenerator: main.o read.o write.o node.o random.o
	gcc $(CFLAGS) -g $^ -o $@ -lm
main.o: main.c read.h
	gcc -c $(CFLAGS) $<
read.o: read.c read.h write.h
	gcc -c $(CFLAGS) $<
write.o: write.c write.h node.h random.h
	gcc -c $(CFLAGS) $< -lm
node.o: node.c node.h
	gcc -c $(CFLAGS) $<
random.o: random.c makros.h random.h
	gcc -c $(CFLAGS) $<
clear:
	rm -f main.o read.o write.o node.o random.o
clearOut:
	rm -f TestGenerator.out
