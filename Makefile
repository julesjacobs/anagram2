anagram: main.cpp Makefile
	g++ main.cpp -o anagram -std=c++17 -O3 -march=native

test: anagram
	./anagram dict/minimal.txt tommarvoloriddle
	./anagram dict/test.txt aimaim

bigtest: anagram
	./anagram dict/pt-Br_dictionary.txt gabrielfarinas > gabrielfarinas.txt
	python3 test.py

time: anagram
	time ./anagram dict/pt-Br_dictionary.txt gabrielfarinas > gabrielfarinas.txt
	time ./anagram dict/pt-Br_dictionary.txt gabrielwernersbachfarinas > gabrielwernersbachfarinas.txt

bigtime: anagram
	time ./anagram dict/us_dictionary.txt gabrielfarinas > gabrielfarinas_us.txt

clean:
	rm anagram