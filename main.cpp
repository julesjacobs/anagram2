#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;
#define $ auto
#define each(i,n) for(int i = 0; i < n; i++)

// TODO: try different memory layout, where the wordlist in the algorithm is represented by an array of pointers instead of an array of hists
// Since hists are large, this may speed up the code, but since it reduces memory locality, it may make it slower.
// if we do this, then we do probably want to pre-filter the word list with respect to target, for memory locality
typedef struct { char hist[26]; string* word; } hist;

hist str_hist(string s, string* word){
    hist out;
    each(i,26) out.hist[i] = 0;
    for($ c : s) out.hist[c - 'a']++;
    out.word = word;
    return out;
}

bool is_sub(hist h1, hist h2){
    each(i,26) if(h1.hist[i] > h2.hist[i]) return false;
    return true;
}

bool allow_multiple_uses = 1;

void anagrams(string sofar, hist target, hist* low, hist* high){
    // select a letter l to branch on
    char l = 0;
    while(l < 26 && target.hist[l] == 0) l++;
    if(l == 26){ cout << sofar << "\n"; return; }

    // permute dict to put impossible words at the end, and decrement high to remove those from consideration
    for($ h = low; h <= high;) if(!is_sub(*h, target)) swap(*h, *high--); else h++;

    // permute dict to put the words that have letter l at the front, between low..next
    $ next = low;
    for($ h = low; h <= high; h++) if(h->hist[l] > 0) swap(*h, *next++);

    // try all the words with letter l (those are now in low..next)
    // this is the main algorithmic optimisation compared to the original
    // we only have to check words that contain l, instead of all words
    // this can reduce the search space by a lot
    // a further optimisation that we don't do here is to choose l intelligently
    // for example, by choosing l such that the number of words to try is minimized
    while(--next >= low){
        $ newsofar = sofar; $ newtarget = target;
        while(is_sub(*next, newtarget)){
            newsofar += *next->word + " ";
            each(i,26) newtarget.hist[i] -= next->hist[i];
            anagrams(newsofar, newtarget, next+1, high);
            if(!allow_multiple_uses) break;
        }
    }
}

int main(int argc, char** argv){
    vector<hist> hists;
    vector<string> words;
    ifstream dict(argv[1]);
    string line;
    while(getline(dict, line)){
        if(line.size()<=2) continue;
        words.push_back(line);
    }
    each(i, words.size()) hists.push_back(str_hist(words[i], &words[i]));
    hist target = str_hist(argv[2],0);
    anagrams("", target, &hists[0], &hists[hists.size()-1]);
}