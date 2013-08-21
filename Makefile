target:
	g++ -ggdb3 -c src/*.cpp
	mv *.o obj/
	g++ -ggdb3 -o overlap-sim obj/*

clean:
	rm obj/*

