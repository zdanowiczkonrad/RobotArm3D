APP=manipulator

__start__: obj ./${APP}
	rm -f core.* core; ./${APP}

# Tworzy katalog "obj" gdy go nie ma
# 
obj:
	mkdir -p obj

./${APP}: obj/start.o obj/manipulator.o obj/lacze_do_gnuplota.o
	g++ -o ./${APP} obj/start.o obj/manipulator.o obj/lacze_do_gnuplota.o

obj/start.o: src/start.cpp inc/lacze_do_gnuplota.hh inc/manipulator.hh
	g++  -Iinc -W -Wall -pedantic -c -o obj/start.o src/start.cpp

obj/manipulator.o: src/manipulator.cpp inc/manipulator.hh
	g++  -Iinc -W -Wall -pedantic -c -o obj/manipulator.o src/manipulator.cpp

obj/lacze_do_gnuplota.o: src/lacze_do_gnuplota.cpp inc/lacze_do_gnuplota.hh
	g++  -Iinc -W -Wall -pedantic -c -o obj/lacze_do_gnuplota.o src/lacze_do_gnuplota.cpp
