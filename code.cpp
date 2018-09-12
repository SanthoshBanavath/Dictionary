//This is the "memory optimised program" which maintains dictionary(a book of words in one language) \
It has basic features like 1.Inserting the data,2.Deleting the data and 3.Fetching the data from the dictionary. \
In addition it has features like 4.reading the all words at once,5.fetching the manually inserted data \
and 6.prefix search(suggestions based on provided prefix)

#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<tr1/unordered_map>
using namespace std;

struct Trie{
	bool EndofWord;
	tr1::unordered_map<char,Trie*> map;
};

Trie* newTrie(){
	Trie* node=new Trie;
	node->EndofWord=false;
	return node;
}

void insert(Trie*& root,const string str){
	if(root==NULL) root=newTrie();
	Trie* temp=root;
	for(int i=0;i<str.length();i++){
		char x=str[i];
		if(temp->map.find(x)==temp->map.end())
			temp->map[x]=newTrie();
		temp=temp->map[x];
	}
	temp->EndofWord=true;
}

bool deleting(Trie*& root,string str,int i){
	if(i==str.length()){
		if(!root->EndofWord) return false;//end reached but it is not made as end of word
		root->EndofWord=false;//make it as not end of word so that the word can't be identified
		return (root->map.size()==0);
	}
	Trie* node=root->map[str[i]];
	if(node==NULL) return false;
	bool shouldDelete=deleting(node,str,i+1);
	if(shouldDelete){
		delete(node->map[str[i]]);
		return (node->map.size()==0);
	}
	return false;
}


bool search(Trie*& root,const string &str){
	if(root==NULL) return false;
	Trie* temp=root;
	for(int i=0;i<str.length();i++){
		temp=temp->map[str[i]];
		if(temp==NULL) return false;
	}
	return temp->EndofWord;
}

void PrintAllEffi(Trie*& root,vector<string>& remWords,string str){
	if(root->map.size()==0){//when it reaches to the leaf node
		if(root->EndofWord){
			//remWords.push_back(str);
			cout<<str<<endl;
		}
		return;
	}
	else if(root->EndofWord){//when it finds a word inbetween
		//remWords.push_back(str);
		cout<<str<<endl;
	}
	tr1::unordered_map<char,Trie*>::iterator it;
	for(it=root->map.begin();it!=root->map.end();it++){
		PrintAllEffi(root->map[it->first],remWords,str+(it->first));
	}//itarate for all possible prefixes
	return;
}
void Print(){
	ifstream file;
	string filename="Ad.Dictionary.txt";
	file.open(filename.c_str());
	string word;
	while(file>>word){
		cout<<word<<endl;
	}
	file.close();
}

void print(vector<string> remWords){
	if(remWords.size()==0){
		cout<<"\n....No words Found.....\n";
		return;
	}
	for(int i=0;i<remWords.size();i++){
		cout<<remWords[i]<<"\n";
	}
	cout<<"\n";
}

vector<string> printAutoSuggestions(Trie*& root,string str){
	Trie* temp=root;
	char ch;
	vector<string> remWords;
	for(int i=0;i<str.length();i++){
		ch=str[i];
		if(temp->map.find(ch)!=temp->map.end()){
			temp=temp->map[ch];
		}
		else return remWords;
	}
	PrintAllEffi(temp,remWords,str);
	return remWords;
}

int main(){
	Trie* root=NULL;
	int n;
	vector<string> remWords;
	vector<string> verified;
	string str1="";
	string word,t,q,filename;
	filename="Dictionary.txt";
	string adfilename="Ad.Dictionary.txt";
	ifstream file;
	file.open(filename.c_str());
	while(file>>word){
		insert(root,word);
	}
	file.close();
	cout<<"...Dictionary is Ready...\n";
	ofstream file1;
	file1.open(adfilename.c_str());
	do{
		cout<<"Choose the option:\n1.Insert\n2.Delete\n3.Search\n4.Dictionary\n5.Manually Inserted words\n6.Search Using prefix\n7.Leave this:";
		cin>>n;
		string str;
		switch(n){
			case 1:
				cout<<"\nEnter the string:";
				cin>>str;
				insert(root,str);
				cout<<"\n....Inserted into dictionary successfully....\n\n";
				file1<<str<<endl;
				break;
			case 2:
				cout<<"\nEnter the string to be deleted:";
				cin>>str;
				if(!search(root,str))
					cout<<"\nThe entered word is not present in the dictionary..!\n";
				else{
					deleting(root,str,0);
					cout<<"\n....Deleted the given word.....\n";
				}
				break;
			case 3:
				cout<<"\nEnter the string:";
				//getline(cin>>std::ws,str);
				cin>>str;
				(search(root,str))?cout<<"\n....Found the given word....\n":cout<<"\n....Not found the given word....\n";
				break;
			case 4:
				PrintAllEffi(root,remWords,str1);
				//print(remWords);
				break;
			case 5:
				Print();
				break;
			case 6:
				cout<<"\nEnter the prefix want to be searched:";
				cin>>str;
				verified=printAutoSuggestions(root,str);
				print(verified);
				break;
			default:
				return 0;
		}
	}while(n!=7);
	file1.close();
	return 0;
}
