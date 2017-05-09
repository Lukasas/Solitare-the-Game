all:
	cd src && make

run:
	cd src && ./hra2017

run-cli:
	cd src && ./hra2017-cli

clean:
	cd src && make clean
	rm -fr ./doc

doxygen:
	doxygen doxconf
