#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<array>
#include<immintrin.h>
#include<unordered_map>

/*
We store histograms of a-z letters as 32 byte SIMD vectors. We represent the target word and dictionary words like that.
The goal is to find a list of dictionary word histograms that sums to the target word histogram.
Given a target word, we can remove any words from the dictionary that are not <= target.
Given a dictionary, we can keep track of the support of each letter (the number of words in the dictionary that contain that letter).
If a letter in the target word becomes unsupported, we have no solution.
If a letter in the target word becomes supported by only 1 word, we have to choose that word.
So pick the letter with lowest support, and try all those words.

To make sure that we generate each anagram exactly once, in only one order, we branch on how many times a word will be used.
After that we remove it from the dictionary, so that it will not be used again. This ensures that we generate each anagram only once.

We can manage the subset of the dictionary by using a vector for the words in the dictionary, with indices (low,high). The currently
active dictionary is then dict[low..high]. To remove words, we permute, and increment low or decrement high. This way we do not have
to allocate anything to create new subsets of the dictionary. We have to be sure to restore low and high properly, and to not permute
in such a way as to disturb parts of the dictionary that are in use by ongoing branches of the backtracking tree.

Algorithm:
==========
1. Remove all words in the dictionary that cannot be used for the target by permuting them to the end, then set high just below those.
2. Find a letter to branch on, and permute all the words that contain that letter just after low.
3. From highest to lowest in that list, branch on how many times we will use that word, and recurse with low set to that new low.

Pseudocode:
===========

void anagrams(hist target, hist* low, hist* high){
    // if target == 0, output the anagram and return
    // permute dict[low..high] to put impossible words at the end, and decrement high to remove those from consideration
    // select a letter l to branch on
    // permute dict[low..high] to put the k words with that letter at the front
    // for newlow = low+k down to low:
    //   word = dict[newlow]
    //   newtarget = target
    //   while(newtarget >= word){
           newtarget -= word
           anagrams(newtarget, newlow, high)
        }
    //
}

void filter(hist target, hist* low, hist* &high){

}

char select_letter(hist target, hist* low, hist* high){

}

void letter_to_front(char letter, list* low, hist* high){

}

TODO:
=====
- Write algorithm in pseudocode
- Work out details
- Write necessary helper functions on SIMD vectors
- Write algorithm in C++
- Test

Intrinsics
==========
_mm256_setzero_si256
_mm256_subs_epi8
_mm256_cmpgt_epi8

_mm256_cmp_epi8
*/

using namespace std;
#define $ auto
#define each(i,n) for(int i = 0; i < n; i++)

typedef __m256i hist;

hist str_hist(string s, int tag){
    hist out = _mm256_setzero_si256();
    $ out2 = (char*)&out;
    for($ c : s) out2[c - 'a'] += 1;
    $ out3 = (int*)&out;
    out3[7] = tag;
    return out;
}

string hist_str(hist h){
    $ h2 = (char*)&h;
    string out;
    each(i,26) each(j,h2[i]) out.push_back('a' + i);
    return out;
}

int hist_tag(hist h){
    $ h2 = (int*)&h;
    return h2[7];
}

hist zeroes(){
    return _mm256_setzero_si256();
}

bool is_zero(hist h_){
    $ h = (char*)&h_;
    each(i,26) if(h[i] != 0) return false;
    return true;
}

// bool is_sub(hist h1_, hist h2_){
//     $ h1 = (char*)&h1_;
//     $ h2 = (char*)&h2_;
//     each(i,26) if(h1[i] > h2[i]) return false;
//     return true;
// }

bool is_sub(hist h1, hist h2){
    $ h = _mm256_cmpgt_epi8(h1, h2);
    return (_mm256_movemask_epi8(h) & 0b11111111111111111111111111) == 0;
}

hist sub(hist h1_, hist h2_){
    $ h1 = (char*)&h1_;
    $ h2 = (char*)&h2_;
    hist out_;
    $ out = (char*)&out_;
    each(i,26) out[i] = h1[i] - h2[i];
    return out_;
}

bool allow_multiple_uses = 0;
vector<string> words;

void anagrams(vector<hist> sofar, hist target, hist* low, hist* high){
    if(is_zero(target)){
        for($ h : sofar) cout << words[hist_tag(h)] << " ";
        cout << "\n";
        return;
    }

    // permute dict to put impossible words at the end, and decrement high to remove those from consideration
    for($ h = low; h <= high;) if(!is_sub(*h, target)) swap(*h, *high--); else h++;

    // select a letter l to branch on
    $ t = (char*)&target;
    char l = 0;
    each(i,26) if(t[i] > 0) l = i;

    // permute dict to put the words that have letter l at the front
    $ next = low;
    for($ h = low; h <= high; h++) if(((char*)h)[l] > 0) swap(*h, *next++);

    while(--next >= low){
        $ newsofar = sofar;
        if(allow_multiple_uses){
            $ newtarget = target;
            while(is_sub(*next, newtarget)){
                newsofar.push_back(*next);
                newtarget = sub(newtarget, *next);
                anagrams(newsofar, newtarget, next+1, high);
            }
        }else{
            newsofar.push_back(*next);
            anagrams(newsofar, sub(target, *next), next+1, high);
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
    vector<hist> sofar;
    anagrams(sofar, target, &hists[0], &hists[hists.size()-1]);
}