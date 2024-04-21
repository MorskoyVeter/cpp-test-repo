#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;
int res;
string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return res=result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

string razbiv(string x){
    string f;
 for(unsigned int j=1;j<x.size();j++) {
  f.push_back(x[j]);
 }
 return f;
}

 vector<string> PoiskMinusSlov (set<string> query_words){
  vector<string> minussl;
     for(string x :query_words ){
         for(unsigned int i=0;i<x.size();i++){
             if(i==0 && x[i]=='-'){
              minussl.push_back(razbiv(x));
             }
          }
     }

     return minussl;
 }


struct Document {
    int id;
    double relevance;
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        double tf=0;
        double raz=words.size();
       for(string x : words){
           int k=0;
      for(string y : words ){
          if(x==y)
              k++;
      }
           tf=k/raz;
       word_to_document_freqs_[x].insert({document_id,tf});
           tf=0;
       }
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        set<string> query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_words);
        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:


   map<string, map<int, double>> word_to_document_freqs_;

    set<string> stop_words_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text)const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    set<string> ParseQuery(const string& text) const {
        set<string> query_words;
        for (const string& word : SplitIntoWordsNoStop(text)) {
            query_words.insert(word);
        }
        return query_words;
    }

    vector<Document> FindAllDocuments(const set<string>& query_words) const {
        vector<string> minussl=PoiskMinusSlov (query_words);
        map<int,double> document_to_relevance;
        vector<Document> matched_documents;
        double n;
        double idf;
        for (const auto& document : query_words) {
            if(!count(minussl.begin(),minussl.end(),document)){
         if(word_to_document_freqs_.count(document)){
          n= word_to_document_freqs_.at(document).size();
          idf=log(res/n);

          for(auto h : word_to_document_freqs_.at(document)){
            
             document_to_relevance.insert({h.first,idf*h.second});
          }
         }
          }
         }
        for(auto x : document_to_relevance){
 matched_documents.push_back({x.first,x.second});
   
  }
           
           return matched_documents;
    }

};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for ( const auto& x: search_server.FindTopDocuments(query)) {
        cout << "{ document_id = " << x.id << ", "
             << "relevance = " << x.relevance << " }"<< endl;
    }
