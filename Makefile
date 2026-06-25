./bin/gol1d: src/gol1d.cxx
	g++ -g src/gol1d.cxx -o bin/gol1d

.PHONY: clean

clean:
	rm -f bin/gol1d
