#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;
#define $ auto
#define each(i,n) for(int i = 0; i < n; i++)

typedef struct { char hist[26]; int tag; } hist;

hist str_hist(string s, int tag){
    hist out;
    each(i,26) out.hist[i] = 0;
    for($ c : s) out.hist[c - 'a'] += 1;
    out.tag = tag;
    return out;
}

bool is_zero(hist h){
    each(i,26) if(h.hist[i] != 0) return false;
    return true;
}

bool is_sub(hist h1, hist h2){
    each(i,26) if(h1.hist[i] > h2.hist[i]) return false;
    return true;
}

bool allow_multiple_uses = 0;
vector<string> words;

void anagrams(string sofar, hist target, hist* low, hist* high){
    if(is_zero(target)){ cout << sofar << "\n"; return; }

    // permute dict to put impossible words at the end, and decrement high to remove those from consideration
    for($ h = low; h <= high;) if(!is_sub(*h, target)) swap(*h, *high--); else h++;

    // select a letter l to branch on
    char l = 0;
    while(target.hist[l] == 0) l++;

    // permute dict to put the words that have letter l at the front
    $ next = low;
    for($ h = low; h <= high; h++) if(h->hist[l] > 0) swap(*h, *next++);

    // try all the words from low..next
    while(--next >= low){
        $ newsofar = sofar;
        $ newtarget = target;
        while(is_sub(*next, newtarget)){
            newsofar += words[next->tag] + " ";
            each(i,26) newtarget.hist[i] -= next->hist[i];
            anagrams(newsofar, newtarget, next+1, high);
            if(!allow_multiple_uses) break;
        }
    }
}

int main(int argc, char** argv){
    vector<hist> hists;
    ifstream dict(argv[1]);
    string line;
    while(getline(dict, line)){
        words.push_back(line);
        hists.push_back(str_hist(line,hists.size()));
    }

    hist target = str_hist(argv[2],0);
    string sofar;
    anagrams(sofar, target, &hists[0], &hists[hists.size()-1]);
}