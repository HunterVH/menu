menu: menu.o cypher.o
	gcc -std=c18 -o menu menu.o cypher.o
	
menu.o: menu.c
	gcc -std=c18 -c -g menu.c
	
cypher.o: cypher.c
	gcc -std=c18 -c -g cypher.c
	
clean:
	rm *.o menu
