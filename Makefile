all: menu permission

menu: menu.o cypher.o directory.o password.o permission.o cracking.o
	gcc -std=c18 -o menu menu.o cypher.o directory.o password.o permission.o cracking.o
	
menu.o: menu.c
	gcc -std=c18 -c -g menu.c

cracking.o: cracking.c
	gcc -std=c18 -c -g cracking.c
	
cypher.o: cypher.c
	gcc -std=c18 -c -g cypher.c
	
directory.o: directory.c
	gcc -std=c18 -c -g directory.c
	
password.o: password.c
	gcc -std=c18 -c -g password.c
	
permission.o: permission.c
	gcc -std=c18 -c -g permission.c
	
permission:
	sudo ./permissions.sh
	
clean:
	rm *.o
	sudo rm menu

