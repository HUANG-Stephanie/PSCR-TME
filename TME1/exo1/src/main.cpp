#include "List.h"
#include <string>
#include <iostream>
#include <cstring>

int main () {

	std::string abc = "abc";
	char * str = new char [4];
	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	str[3] = '\0';
	int i = 0;

	//FAUTE : strcmp compare l'égalité entre 2 chaines de caractère avec le caractère de fin de chaine or la chaine str ne possède pas de caractère de fin de chaine
	if (! strcmp (str, abc.c_str())) {
		std::cout << "Equal !" << std::endl;
	}
 
	pr::List list;
	list.push_front(abc);
	list.push_front(abc);

	std::cout << "Liste : " << list << std::endl;
	std::cout << "Taille : " << list.size() << std::endl;

	//FAUTE : rentre dans la boucle 3 fois et non 2 fois à cause du type size_t
	// Affiche à l'envers
	for (i= list.size()-1 ; i >= 0 ; i--) {
		std::cout << "elt " << i << ": " << list[i] << std::endl;
	}
	
	//FAUTE : pas besoin de libérer les char de la chaine

	// liberer les char de la chaine
	/*for (char *cp = str ; *cp ; cp++) {
		delete cp;
	}
	*/

	//FAUTE : on doit ajouter [] si on veut liberer un tableau 
	// et la chaine elle meme
	delete [] str;

	return 0;
}
