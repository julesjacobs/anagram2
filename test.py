target = "gabrielfarinas"
out = "gabrielfarinas.txt"
dictfile = "dict/pt-Br_dictionary.txt"

norm = lambda w: "".join(sorted(w))


words = list(open(dictfile))
wordset = set(norm(w.strip()) for w in words)
print(len(wordset))
print(len(words))
anagrams = list(open(out))

for anagram in anagrams:
    for w in anagram.strip().split(" "):
        if norm(w) not in wordset:
            print("ERROR: '%s' in '%s' not in dict" % (w, anagram))

for anagram in anagrams:
    if norm("".join(w for w in anagram.strip().split(" "))) != norm(target):
        print("ERROR: '%s' is not an anagram of '%s'" % (anagram, target))

nanagrams = [" ".join(sorted(ana.strip().split(" "))) for ana in anagrams]
if(len(set(nanagrams)) != len(nanagrams)):
    print("ERROR: non uniques")