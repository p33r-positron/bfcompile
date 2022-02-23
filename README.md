# bfcompile
Transpilateur/Compilateur BrainFuck vers Exécutable via traduction en C.  

# Installation

`gcc bfcompile.c -o bfcompile`  
Remplacez gcc par votre compilateur favori.  

# Utilisation

`bfcompile <fichier.bf> <nomExecutableFinal>`  
Exemple: `bfcompile test.bf test` (Le .exe devrait être ajouté automatiquement)  

# Fonctionnement

BrainFuck -> bfcompile -> C -> GCC/CLANG/TCC -> Exécutable/Binaire  

# Dépendences

Il faut avoir GCC ou CLANG ou TCC installé, et dans le PATH.  

# Trucs à Fixer

Rendre possible la suppression automatique du fichier "temporaire" avec un argument CLI.  
Utiliser switch à la place de if-else  
