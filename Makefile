main: main.o sparse_matrix_exceptions.o test_class.o
	g++ main.o sparse_matrix_exceptions.o test_class.o -o main  --std=c++98

main.o: main.cpp SparseMatrix.h
	g++ -c main.cpp -o main.o --std=c++98

test_class.o: test_class.cpp
	g++ -c test_class.cpp -o test_class.o --std=c++98

sparse_matrix_exceptions.o: sparse_matrix_exceptions.cpp
	g++ -c sparse_matrix_exceptions.cpp -o sparse_matrix_exceptions.o  --std=c++98


.PHONY:
clean:
	rm main *.o