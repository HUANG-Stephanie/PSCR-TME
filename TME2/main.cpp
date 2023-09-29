#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <cstring>

int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("../TME2/WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	vector<string> vec1;
	vector<string>::iterator it1;
	vector<pair<string,int>> vec2;
	vector<pair<string,int>>::iterator it2;
	size_t nombre_different = 0;
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

		// word est maintenant "tout propre"
		/*
		 if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		 */
		nombre_lu++;

		//Nombre differents
		it1 = find(vec1.begin(),vec1.end(),word);
		if(it1 == vec1.end()){
			nombre_different++;
			vec1.push_back(word);
		}

		//Occurrences
		it2 = find(vec2.begin(),vec2.end(),word);
		if(it2 == vec2.end()){
			vec2.push_back(make_pair(word,1));
		}else{
			for(auto &k : vec2){
				if(k.first == word){
					k.second++;
					break;
				}
			}
		}
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;
    cout << "Nombre de mots differents = " << nombre_different <<endl;
    cout << "Occurrences = " << endl;
    /*for(int i=0;i<vec2.size();i++){
		if(strcmp(vec2[i].first,"war") == 0){
			cout << vec2[i].first << " : " << vec2[i].second << endl;
		}
    }
    */

    return 0;
}


