#include "preprocesador.h"
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

unordered_set<string> stopwords = {
    "i","me","my","myself","we","our","ours","ourselves",
    "you","your","yours","yourself","yourselves",
    "he","him","his","himself","she","her","hers","herself",
    "it","its","itself","they","them","their","theirs","themselves",
    "what","which","who","whom","this","that","these","those",
    "am","is","are","was","were","be","been","being",
    "have","has","had","having",
    "do","does","did","doing",
    "a","an","the",
    "and","but","if","or","because","as","until","while",
    "of","at","by","for","with","about","against","between",
    "into","through","during","before","after","above","below",
    "to","from","up","down","in","out","on","off","over","under",
    "again","further","then","once",
    "here","there","when","where","why","how",
    "all","any","both","each","few","more","most","other",
    "some","such","no","nor","not","only","own","same",
    "so","than","too","very","can","will","just","should"
};

string normalizar(const string& texto) {
    string r = "";

    for (int i = 0; i < texto.size(); i++) {
        char c = texto[i];

        if (c >= 'A' && c <= 'Z') {
            c = c + 32;
        }

        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ') {
            r += c;
        }
    }

    return r;
}

vector<string> tokenizar(const string& texto) {
    vector<string> palabras;
    string actual = "";

    for (int i = 0; i < texto.size(); i++) {
        if (texto[i] == ' ') {

            if (actual != "") {
                if (stopwords.count(actual) == 0) {
                    palabras.push_back(actual);
                }
                actual = "";
            }

        } else {
            actual += texto[i];
        }
    }

    if (actual != "") {
        if (stopwords.count(actual) == 0) {
            palabras.push_back(actual);
        }
    }

    return palabras;
}