#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	if(argc != 3) //Si il n'y a pas spécifiquement 3 arguments...
		return fprintf(stderr, "Utilisation: bfcompile <file.bf> <binaryName>\r\n")-47; //48 caractères - 47 = 1, code d'erreur 1 :)

	FILE *in = 0, *out = 0; //Création de pointeurs vides pour les fichiers

	in = fopen(argv[1], "r"); //Assignation du pointeur "in" vers le fichier d'entrée en mode lecture, pour pouvoir le lire
	out = fopen("tmpfile.c", "w"); //Assignation du pointeur "out" vers le fichier de sortie temporaire en mode écriture, détruisant le fichier
	#if defined(WIN32) || defined(_WIN32)
		system("chcp 65001 > nul"); //Windows et les accents... Change de "page de code" pour les accents (chcp) mais discrètement (> nul)
	#endif

	if(in == 0) //Si in pointe toujours vers 0/NULL...
		return fprintf(stderr, "Erreur pendant l'ouverture du fichier d'entrée :(\r\n")-50; //...Erreur ! 51 caractères - 50 = 1, code d'erreur 1 :)
	if(out == 0) //Si out pointe toujours vers 0/NULL...
		return fprintf(stderr, "Erreur pendant la suppression du fichier de sortie :(\r\n")-54; //...Erreur ! 55 caractères - 54 = 1, code d'erreur 1 :)

	out = 0; //Ré-assignation du pointeur "out" vers 0/NULL
	out = fopen("tmpfile.c", "a"); //Assignation du pointeur "out" vers le fichier de sortie temporaire en mode "ajout"

	if(out == 0) //Si out pointe toujours vers 0/NULL...
		return fprintf(stderr, "Erreur pendant l'ouverture du fichier de sortie :(\r\n")-51; //...Erreur ! 52 caractères - 51 = 1, code d'erreur 1 :)

	//Écriture de la base nécéssaire dans le fichier temporaire, un char prends un octet, donc c'est bien 135 car on veut pas du null terminator caché.
	fwrite("#include <stdio.h>\n\n//Auto-Généré par bfcompile\n\nint main(void)\n{\n\tunsigned char memory[30000] = {0};\n\tunsigned char* ptr = memory;\n", sizeof(char), (size_t)135, out);
	char caractere = 0; //Assignation de caractere vers le caractère Nº0 de la table ASCII (Null Terminator)
	do
	{
		caractere = fgetc(in); //Ré-assignation de caractere vers le prochain caractère du fichier d'entrée
		if(caractere == '>') //Si le caractere est un symbole supérieur/chevron vers la droite/fermant...
			fwrite("\tptr++;\n", sizeof(char), 8, out); //...on incrémente le pointeur/déplace de 1 le pointeur vers la droite !
		else if(caractere == '<') //Si le caractere est un symbole inférieur/chevron vers la gauche/ouvrant...
			fwrite("\tptr--;\n", sizeof(char), 8, out); //...on décrémente le pointeur/déplace de 1 le pointeur vers la gauche !
		else if(caractere == '+') //Si caractere est un plus...
			fwrite("\t(*ptr)++;\n", sizeof(char), 11, out); //...on incrémente la valeur du pointeur !
		else if(caractere == '-') //Si caractere est un moins...
			fwrite("\t(*ptr)--;\n", sizeof(char), 11, out); //...on décrémente la valeur du pointeur !
		else if(caractere == '.') //Si caractere est un point...
			fwrite("\tputchar(*ptr);\n", sizeof(char), 16, out); //...on affiche le caractère de la table ASCII corespondant à la valeur actuelle du pointeur !
		else if(caractere == ',') //Si caractere est une virgule...
			fwrite("\t*ptr = getchar();\n", sizeof(char), 19, out); //...on prend un caractère de l'utilisateur et on remplace la valeur actuelle du pointeur avec !
		else if(caractere == '[') //Si caractere est un crochet ouvrant...
			fwrite("\twhile(*ptr);\n\t{\n", sizeof(char), 17, out); //...on crée une boucle qui dure tant que la valeur du pointeur n'est pas 0 !
		else if(caractere == ']') //Si caractere est un crochet fermant...
			fwrite("\t}\n", sizeof(char), 3, out); //...on marque la fin des actions de la boucle !

		//Dans les autres cas, ça ne peut pas être une instruction brainfuck, donc c'est probablement un commentaire.
	}
	while(caractere != EOF); //5 = EOF ("Fin de fichier"), tant que le caractère n'est pas EOF, on lit !

	//Écriture de la fin nécéssaire dans le fichier temporaire.
	fwrite("\n\treturn 0;\n}\n", sizeof(char), 14, out);

	//On ferme tous les fichiers ouverts précédémment
	if(fclose(in) == EOF) //Si le fichier d'entrée n'est toujours pas fermé...
		return fprintf(stderr, "Erreur pendant la fermeture du fichier d'entrée.\r\n"-49); //...Erreur ! 50 caractères - 49 = 1, code d'erreur 1 :)
	if(fclose(out) == EOF) //Si le fichier de sortie n'est toujours pas fermé...
		return fprintf(stderr, "Erreur pendant la fermeture du fichier de sortie.\r\n"-50); //...Erreur ! 51 caractères - 50 = 1, code d'erreur 1 :)

	char compilation[20+strlen(argv[2])]; //On crée une chaîne de caractère assez grande pour garder la commande de compilation

	printf("Essai de compilation avec GCC...\r\n");
	sprintf(compilation, "gcc   tmpfile.c -o %s", argv[2]); //On met la commande de compilation version GCC dans compilation
	if(system(compilation) == 0) //Si la commande renvoie 0 (Code d'erreur signifiant l'absence d'erreurs)...
		printf("Réussi !\r\n"); //...fin !
	else //Sinon:
	{
		printf("Raté...\r\nEssai de compilation avec CLANG...\r\n");
		sprintf(compilation, "clang tmpfile.c -o %s", argv[2]); //On met la commande de compilation version CLANG dans compilation
		if(system(compilation) == 0) //Si la commande renvoie 0 (Code d'erreur signifiant l'absence d'erreurs)...
			printf("Réussi !\r\n"); //...fin !
		else //Sinon:
		{
			printf("Raté...\r\nEssai de compilation avec TCC...\r\n");
			sprintf(compilation, "tcc   tmpfile.c -o %s", argv[2]); //On met la commande de compilation version TCC dans compilation
			if(system(compilation) == 0) //Si la commande renvoie 0 (Code d'erreur signifiant l'absence d'erreurs)...
				printf("Réussi !\r\n"); //...fin !
			else //Sinon, message d'erreur final !
				return fprintf(stderr, "Désolé, aucune compilation n'a réussi, la cause probable est que le développeur est un idiot ou que vous ne possèdez ni GCC, ni TCC, ni CLANG.\r\n")-143;
		}
	}

	return 0; //0 -> Pas d'erreur ! :)
}