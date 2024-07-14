#include "trie.h"
#include <iostream>
#include <fstream>
#include <vector>

Trie::Trie()
{
	root = NULL;
}

// Returns new trie node (initialized to NULLs)
struct TrieNode* Trie::getNode(void)
{
	TrieNode* A = new TrieNode;
	A->isWordEnd = 0;
	A->score = 0;
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		A->children[i] = NULL;
	}

	return A;	
}

// If not present, inserts key into trie. If the
// key is prefix of trie node, just marks leaf node
void Trie::insert(struct TrieNode* root, const std::string key)
{
	TrieNode* A, *B;
	int i,n;
	if(root == NULL) {
		A = getNode();
		root = A;
		i = 0; n = 0;
		while(key[i] != 0) {
			B = getNode();
			n = CHAR_TO_INDEX(key[i]);
			if(key[i] == ' ') n = CHAR_TO_INDEX('{');
			A->children[n] = B;
			A = B;
			i++;
		}
		A->isWordEnd = true;
		A->score = 1;
		return;
	}

	A = root;
	n = CHAR_TO_INDEX(key[0]);
	i = 0;
	while(A->children[n] != NULL){
		A = A->children[n];
		i++;
		n = CHAR_TO_INDEX(key[i]);

		if(key[i] == ' ') n = CHAR_TO_INDEX('{');
	}

	while(key[i] != 0) {
		B = getNode();
		n = CHAR_TO_INDEX(key[i]);
		if(key[i] == ' ') n = CHAR_TO_INDEX('{');
		A->children[n] = B;
		A = B;
		i++;
	}
	A->isWordEnd = true;
	A->score += 1;
	return;	
}

bool Trie::search(struct TrieNode *root, std::string key)
{
	if(root == NULL) return false;
	TrieNode *A = root, *B;
	int i = 0, n = 0;
	while(key[i] != 0) {
		n = CHAR_TO_INDEX(key[i]);
		if(key[i] == ' ') n = CHAR_TO_INDEX('{');
		if(A->children[n] == NULL){
			break;
		}
		A = A->children[n];
		i++;
	}
	if(i == key.size()) return true;
	return false;
}

bool Trie::increasescore(struct TrieNode *root, std::string key){
	if(root == NULL) return false;
	TrieNode *A = root, *B;
	int i = 0, n = 0;
	while(key[i] != 0) {
		n = CHAR_TO_INDEX(key[i]);
		if(key[i] == ' ') n = CHAR_TO_INDEX('{');
		if(A->children[n] == NULL){
			break;
		}
		A = A->children[n];
		i++;
	}
	if(i == key.size() && A->isWordEnd){
		A->score += 1;
		return true;
	}
	return false;
}

// Returns 0 if current node has a child
// If all children are NULL, return 1.
bool Trie::isLastNode(struct TrieNode* root)
{
	if(root->isWordEnd == 1) return true;
	return false;
}

// Recursive function to print auto-suggestions for given
// node.
void Trie::suggestionsRec(struct TrieNode* root,
					std::string currPrefix, 
					std::vector<std::pair<std::string, int>>& possible)
{
	// found a std::string in Trie with the given prefix
	bool flag = false;

	TrieNode *A = root, *B;
	int i = 0, n = 0;
	while(currPrefix[i] != 0) {
		n = CHAR_TO_INDEX(currPrefix[i]);
		if(currPrefix[i] == ' ') n = CHAR_TO_INDEX('{');
		if(A->children[n] == NULL){
			break;
		}
		A = A->children[n];
		i++;
	}

	std::string arr = currPrefix, arr_prime, last_before, last;
	i = 0;
	if(A->isWordEnd == true){
		while(arr[i] != 0){
			if(arr[i] == ' '){
				last_before = last;
				last = "";
			} else {
				last += arr[i];
			}
			i++;
		}
		possible.push_back(std::make_pair(last, A->score));
	}

	for(int i = 0; i < ALPHABET_SIZE; i++) {
		if(A->children[i] != NULL) flag = true;
	}
	if( flag == false) {
		return;
	}

	for(int i = 0; i < ALPHABET_SIZE; i++) {
		if(A->children[i] != NULL) {
			arr_prime = arr + char(i + (int)'a');
			suggestionsRec(root, arr_prime, possible);
		}
	}
}

// print suggestions for given query prefix.
int Trie::printAutoSuggestions(TrieNode* root, const std::string query)
{
	if(root == NULL) return -1;

	std::string last_before = "";
	std::string last = "";
	int i = 0, n = query.size();
	while(i < n){
		if(query[i] == '#'){
			break;
		}
		if(query[i] == ' '){
			last_before = last;
			last = "";
		} else {
			last += query[i];
		}
		i++;
	}
	std::string temp = last_before + " " + last;
	
	n = CHAR_TO_INDEX(temp[0]);
	if(root->children[n] == NULL){
		return 0;
	}

	TrieNode *A = root, *B;
	i = 0; n = 0;
	while(temp[i] != 0) {
		n = CHAR_TO_INDEX(temp[i]);
		if(temp[i] == ' ') n = CHAR_TO_INDEX('{');
		if(A->children[n] == NULL){
			break;
		}
		A = A->children[n];
		i++;
	}
	if(i != temp.size()) return 0;

	std::vector<std::pair<std::string, int>> possible;

	suggestionsRec(root, temp, possible);

	std::sort(possible.begin(), possible.end(),
	 [&](auto& a, auto& b){
		return a.second > b.second;
	 });

	// print words in descending order of score
	for(i = 0 ; i < possible.size(); i++){
		std::cout << possible[i].first << std::endl;
	}
	return 1;

}

// Process the file "lorem.txt" to insert the words in lorem.txt and store the relevant context as needed.
void Trie::processContext()
{
	std::string first;
	std::string second;
	std::string temp;
	std::ifstream file("temp.txt");

	file >> first;
	while(file >> second){
		temp = first + " " + second;
		if(search(root, temp)){
			if(! increasescore(root, temp)) std::cout << "some problem" << std::endl;
		} else {
			// std::cout << temp << std::endl;
			insert(root, temp);
		}
		first = second;
	}
}
