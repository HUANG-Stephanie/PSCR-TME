#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include "hash_table.hpp"

int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("WarAndPeace.txt");

	vector<string> words;
	vector<pair<string,int>> wpt;
	unordered_map<string,int> hash_table(100000);
	vector<pair<string,int>> copy;
	vector<pair<string,int>> copy2;
	unordered_map<int,forward_list<string>> hash_table_inv(100000);
	

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);
	
		// Q2
		// on ajoute le mot à la liste si il n'y est pas déjà
		/*if (find(words.begin(), words.end(), word) == words.end())
		{
			words.push_back(word);		
		}*/

		// Q3
		// on ajoute war peace ou toto au vecteur wpt, s'il y est déjà on incrémente le compteur
		/*if (word == "war" || word == "peace" || word == "toto")
		{
			bool found = false;
			for (auto &w : wpt)
			{
				if (w.first == word)
				{
					w.second++;
					found = true;
					break;
				}
			}
			if (!found)
			{
				wpt.push_back(make_pair(word,1));
			}
		}*/

		// Q6
		// on ajoute les mots à la table de hachage hash_table, s'il y est déjà on incrémente le compteur 
		unordered_map<std::string, int>::iterator it = hash_table.find(word);
		if (it != nullptr){
			(*it).second++;
		}
		else{
			hash_table.insert(make_pair(word,1));
		}
	
		// word est maintenant "tout propre"
		/*if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		nombre_lu++;*/
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    //cout << "Found a total of " << nombre_lu << " words." << endl;

	// Q2
	// on affiche le nombre de mots différents trouvés
	//cout << "Found " << words.size() << " different words." << endl;

	// Q3 
	// on affiche le nombre de fois que les mots war, peace et toto apparaissent
	/*for (auto w : wpt)
	{
		cout << "Found " << w.second << " times the word " << w.first << endl;
	}*/
	// la compléxité algorithmique de la recherche est de O(n) car on parcourt le vecteur wpt pour chaque mot lu
	// il serait plus efficace d'utiliser une table de hashage pour stocker les mots et leur nombre d'apparition

	cout << "---------------" << endl;
	// Q6
	// on affiche le nombre de fois que les mots war, peace et toto apparaissent
	unordered_map<std::string, int>::iterator it1 = hash_table.find("war");
	if (it1 != nullptr)
	{
		cout << "Found " << (*it1).second << " times the word war" << endl;
	}
	unordered_map<std::string, int>::iterator it2 = hash_table.find("peace");
	if (it2 != nullptr)
	{
		cout << "Found " << (*it2).second << " times the word peace" << endl;
	}
	unordered_map<std::string, int>::iterator it3 = hash_table.find("toto");
	if (it3 != nullptr)
	{
		cout << "Found " << (*it3).second << " times the word toto" << endl;
	}
	cout << "---------------" << endl;
	// la compléxité algorithmique de la recherche est de O(1) car on utilise une table de hashage
	// il est donc plus efficace d'utiliser une table de hashage pour stocker les mots et leur nombre d'apparition

	// copie de la table de hashage dans un vecteur
	for(unordered_map<std::string, int>::iterator it = hash_table.begin(); it != hash_table.end(); ++it){
		copy.push_back(make_pair((*it).first,(*it).second));
	}

	// tri du vecteur
	sort(copy.begin(),copy.end(),[](const pair<string,int> &a, const pair<string,int> &b){return a.second > b.second;});
	
	// affichage des 10 premiers mots
	for (size_t i = 0; i < 10; i++)
	{
		cout << "Found " << copy[i].second << " times the word " << copy[i].first << endl;
	}
	cout << "---------------" << endl;

	//TME3 Q5-6
	for(unordered_map<std::string, int>::iterator _it = hash_table.begin(); _it != hash_table.end(); ++_it){
		copy2.push_back(make_pair((*_it).first, (*_it).second));
	}

	sort(copy2.begin(),copy2.end(),[](const pair<string,int> &a, const pair<string,int> &b){return a.second > b.second;});

	for(size_t i = 0; i < 10; i++){
		cout << "Found " << copy2[i].second << " times the word " << copy2[i].first << endl;
	}

	//TME3 Q1
	cout << "---------------" << endl;
	cout << "Fonction size()" << endl;
	cout << hash_table.size() << endl;
	cout << "---------------" << endl;

	//TME3 Q2
	cout << "Fonction count_if_equal()" << endl;
	cout << hash_table.count("the") << " éléments égaux à 'the' " << endl;
	cout << "---------------" << endl;
	
	//TME3 Q7
	for(unordered_map<std::string, int>::iterator it = hash_table.begin(); it != hash_table.end(); ++it){
		hash_table_inv[(*it).second].push_front((*it).first);
	}
	cout << "Affichage N occurrances" << endl;
	const size_t N = 5;
	for(auto e: hash_table_inv[N]){
		cout << e << endl;
	}

    return 0;
}


