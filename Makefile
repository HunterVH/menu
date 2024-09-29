menu: menu.o cypher.o directory.o
	gcc -std=c18 -o menu menu.o cypher.o directory.o
	
menu.o: menu.c
	gcc -std=c18 -c -g menu.c
	
cypher.o: cypher.c
	gcc -std=c18 -c -g cypher.c
	
directory.o: directory.c
	gcc -std=c18 -c -g directory.c
	
clean:
	rm *.o menu
