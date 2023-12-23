SRCS = $(wildcard src/*.java)

all: $(SRCS)
	javac -g -d build $^

run: all
	java -Djava.util.logging.config.file=src/logging.properties -classpath build Main

clean:
	rm -rf build/
	rm -f LNR.txt LOT.txt NLR.txt


.PHONY: all run clean
