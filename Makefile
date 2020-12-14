compile:
	gcc -Wall -g security_audit.c auditdir.c utilities.c -o main.out
	
valgrind:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./main.out
		 
run:
	./main.out
	
clean:
	rm *.out
	echo 'Cleaning done'
