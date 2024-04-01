build:
	gcc *.c -o tema1

run: build
	./tema1

clean:
	rm -rf tema1