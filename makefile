all:
	cd src && make

clean:
	cd src && make clean
	rm -r ./doc

doxygen:
	doxygen doxconf
