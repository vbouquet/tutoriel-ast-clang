# Exemples d'utilisation de clang

## Documentations
- [Documentation officiel de l'API clang](https://clang.llvm.org/doxygen/namespaceclang.html#details)
- [Documentation officiel sur les différents outils de clang](https://clang.llvm.org/docs/index.html)
- [Documentation officiel des Ast matcher](http://clang.llvm.org/docs/LibASTMatchersReference.html)
- [Des exemples d'utilisations de LLVM et CLANG](https://github.com/eliben/llvm-clang-samples)
- [Clang ASTMatcher, ASTVisteur et réécriture de code](https://eli.thegreenplace.net/2014/07/29/ast-matchers-and-clang-refactoring-tools)

## Pré-requis
1. Nous utilisons vagrant pour installer les dépendances dans une machine virtuelle, vous pouvez l'obtenir ici : [Télécharger vagrant](https://www.vagrantup.com/downloads.html)
2. Nous utilisons virtualbox pour déployer la machine virtuelle, vous pouvez l'obtenir ici: [Télécharger Virtualbox](https://www.virtualbox.org/wiki/Downloads)
3. Vous pouvez utiliser l'outil de gestion de version git pour télécharger ce dépôt ou bien le faire manuel.

## Installation
1. Télécharger le dépôt manuellement ou lancez la commande ```git clone https://github.com/vbouquet/tutoriel-ast-clang.git```
2. Ouvrez un terminal (**Attention! En mode administrateur si vous êtes sur windows**) et situez vous dans le dossier du répertoire ~/[VOTRE_CHEMIN]/tutoriel-ast-clang
3. Déployez la machine virtuelle avec vagrant (**nécessite une connexion internet**)```vagrant up``` puis redémarrer la à la fin de l'exécution avec la commande ```vagrant reload```. Ne paniquez pas si vous voyez du texte rouge défiler, ceci est normal.
4. Entrez dans la machine virtuelle avec la commande ```vagrant ssh``` et vous êtes maintenant isolé de votre système et prêt à travailler avec clang.
5. Pour vérifier que clang a été correctement installé et que le projet a compilé lancez les commandes:

```
cd /vagrant
build/clang_ast_analysis exemples/file.c -- -I/usr/include/clang/3.8/include
```

Vous devriez voir apparaître la sortie suivante
```
#include stdio.h
#include stdlib.h
Fonction définie: somme
Fonction appelée: a
Fonction appelée: b
Fonction définie: division
Fonction appelée: a
Fonction appelée: b
Fonction définie: hello_world
Fonction appelée: printf
Fonction définie: main
Fonction appelée: hello_world
Fonction appelée: somme
Fonction appelée: a
Fonction appelée: b
Fonction appelée: division
Fonction appelée: a
Fonction appelée: b
```

Votre machine virtuelle partage avec votre système d'explotation le dossier tutoriel-ast-clang. Vous pouvez donc faire toutes les modifications que vous souhaitez à l'intérieur, et celles ci se récupercuterons dans la machine virtuelle dans le dossier /vagrant.

Accédez au code source avec éditeur de texte ou votre IDE favori depuis votre système et commencez à coder.

## Commandes

```
# Démarrrer la VM
vagrant up

# Eteindre la VM
vagrant halt

# Supprimer la VM
vagrant destroy

# Se connecter en ssh à la VM
vagrant ssh

# Lancez une commande depuis votre système
vagrant ssh -c "build/clang_ast_analysis exemples/<votre_fichier_a_analyser> -- -I/usr/include/clang/3.8/include"
```
