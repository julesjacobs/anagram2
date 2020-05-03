anagram: main.cpp
	g++ main.cpp -o anagram -std=c++17 -mavx2

test: anagram
	./anagram dict/minimal.txt tommarvoloriddle
	./anagram dict/test.txt aimaim

bigtest: anagram
	./anagram dict/pt-Br_dictionary.txt gabrielfarinas > gabrielfarinas.txt
	python3 test.py

time: anagram
	time ./anagram dict/pt-Br_dictionary.txt gabrielfarinas

clean:
	rm anagram