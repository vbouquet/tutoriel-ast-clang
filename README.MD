# Exemples d'utilisation de clang

## Documentations
- [Documentation officiel de l'API clang](https://clang.llvm.org/doxygen/namespaceclang.html#details)
- [Documentation officiel sur les différents outils de clang](https://clang.llvm.org/docs/index.html)
- [Documentation officiel des Ast matcher](http://clang.llvm.org/docs/LibASTMatchersReference.html)
- [Des exemples d'utilisations de LLVM et CLANG](https://github.com/eliben/llvm-clang-samples)
- [Clang ASTMatcher, ASTVisteur et réécriture de code](https://eli.thegreenplace.net/2014/07/29/ast-matchers-and-clang-refactoring-tools)
- [Liste des noeuds déclaration avec clang](https://clang.llvm.org/doxygen/classclang_1_1Decl.html)
- [Liste des noeuds statement avec clang](https://clang.llvm.org/doxygen/classclang_1_1Stmt.html)

## Pré-requis
1. Nous utilisons vagrant pour installer les dépendances dans une machine virtuelle, vous pouvez l'obtenir ici : [Télécharger vagrant](https://www.vagrantup.com/downloads.html)
2. Nous utilisons virtualbox pour déployer la machine virtuelle, vous pouvez l'obtenir ici: [Télécharger Virtualbox](https://www.virtualbox.org/wiki/Downloads)
3. Vous pouvez utiliser l'outil de gestion de version git pour télécharger ce dépôt ou bien le faire manuellement en cliquant sur le bouton vert "cloner ou télécharger" en haut à droite du dépôt.

## Installation avec vagrant (conseillé)
1. Télécharger le dépôt manuellement à l'addresse https://github.com/vbouquet/tutoriel-ast-clang/archive/master.zip ou cloner le avec git ```git clone https://github.com/vbouquet/tutoriel-ast-clang.git```
2. Ouvrez un terminal (**Attention! En mode administrateur si vous êtes sur windows**) et situez vous dans le dossier du répertoire ~/[VOTRE_CHEMIN]/tutoriel-ast-clang (vous pouvez d'ailleurs renomer le dossier si vous le souhaitez).
3. Déployez la machine virtuelle avec vagrant (**nécessite une connexion internet**) ```vagrant up``` puis redémarrer la à la fin de l'exécution avec la commande ```vagrant reload```. Ne paniquez pas si vous voyez du texte rouge défiler, ceci est normal.
4. Entrez dans la machine virtuelle avec la commande ```vagrant ssh``` et vous êtes maintenant isolé de votre système et prêt à travailler avec clang.
5. Pour vérifier que clang a été correctement installé et que le projet a compilé lancez les commandes:

ASTMatcher:
```
cd /vagrant
build/clang_ast_matcher exemples/file.c -- -I/vagrant/libs/clang_llvm/lib/clang/3.9.1/include/
```

Vous devriez voir apparaître la sortie suivante pour l'AST Matcher
```
#include stdio.h
Fonction définie: somme
Fonction définie: division
Fonction définie: hello_world
Fonction appelée: printf
Fonction définie: main
Fonction appelée: hello_world
Fonction appelée: somme
Fonction appelée: division
```

ASTVisitor:
```
cd /vagrant

# Executable de l'AST Visitor (pour la réécriture de code)
build/clang_ast_visitor exemples/file.c -- -I/vagrant/libs/clang_llvm/lib/clang/3.9.1/include/
```

Vous devriez voir apparaître un message d'erreur, ceci est normal. Vérifiez qu'un fichier file.c est présent dans le dossier output et qu'une fonction 'somme_mglwnafh_Cthulhu_Rlyeh_wgah_nagl_fhtagn' est bien présente.

Votre machine virtuelle partage avec votre système d'explotation le dossier tutoriel-ast-clang. Vous pouvez donc faire toutes les modifications que vous souhaitez à l'intérieur, et celles ci se récupercuterons dans la machine virtuelle dans le dossier /vagrant.

Accédez au code source avec éditeur de texte ou votre IDE favori depuis votre système et commencez à coder.

**Conseil**: Si vous n'utilisez que l'AST visitor ou l'AST Matcher nous vous conseillons de mettre en commantaire les lignes indiqués vers la fin du fichier **Makefile** pour accélerer la compilation du projet.

## Installation manuelle

Nous vous conseillons d'utiliser cette méthode que si vous ne pouvez pas utiliser vagrant ou que vous n'êtes pas administrateur de votre système.
1. Télécharger de linux, vous vous conseillons d'utiliser ubuntu 16.04 (pour des raisons de dépendances avec les outils utilisés), vous pouvez en utiliser une autre éventuellement mais nous ne garantissons rien.
Si vous souhaitez un système léger nous vous conseillons lubuntu: http://cdimages.ubuntu.com/lubuntu/releases/16.04/release/lubuntu-16.04-desktop-amd64.iso
Si vous avez déjà un système d'exploitation linux natif, passé à l'étape 4.
2. Suivez le guide d'installation d'une machine virtuelle disponible [ici](https://docs.oracle.com/cd/E26217_01/E35193/html/qs-create-vm.html). A la figure 6.9, pensez à remplacer l'iso par celui que vous venez de télécharger.
3. Lancer la machine virtuelle et suivez le guide d'instllation. Choissisez vous même votre nom d'utilisateur et votre mot de passe, assurez vous d'avoir un accès root avec votre utilisateur (administrateur).
4. Télécharger git depuis le dépôt aptitude ```sudo apt-get install git ```
5. Cloner le dépôt du tutoriel de clang et déplacer le dans le dossier vagrant
```
cd
git clone https://github.com/vbouquet/tutoriel-ast-clang.git
sudo mv tutoriel-ast-clang /vagrant
cd /vagrant
```
6. Installer toutes les dépendances avec la commande ```script/install.sh```
7. Vérifier que l'installation s'est correctement effectué en lançant les commandes suivantes:

ASTMatcher:
```
cd /vagrant

build/clang_ast_matcher exemples/file.c -- -I/vagrant/libs/clang_llvm/lib/clang/3.9.1/include/
```

Vous devriez voir apparaître la sortie suivante pour l'AST Matcher
```
#include stdio.h
Fonction définie: somme
Fonction définie: division
Fonction définie: hello_world
Fonction appelée: printf
Fonction définie: main
Fonction appelée: hello_world
Fonction appelée: somme
Fonction appelée: division
```

ASTVisitor:
```
cd /vagrant

build/clang_ast_visitor exemples/file.c -- -I/vagrant/libs/clang_llvm/lib/clang/3.9.1/include/
```

Vous devriez voir apparaître un message d'erreur, ceci est **normal**. Vérifiez qu'un fichier file.c est présent dans le dossier output et qu'une fonction 'somme_mglwnafh_Cthulhu_Rlyeh_wgah_nagl_fhtagn' est bien présente.

## Commandes (utilitaire)

```
# Démarrrer la VM
vagrant up

# Redémarrer la VM
vagrant reload

# Eteindre la VM
vagrant halt

# Supprimer la VM
vagrant destroy

# Obtenir des informations sur l'état de la VM
vagrant status

# Se connecter en ssh à la VM
vagrant ssh

# Compiler vos fichiers clang
cd /vagrant
make

# Exécuter l'AST Matcher
build/clang_ast_matcher exemples/file.c -- -I/vagrant/libs/clang_llvm/lib/clang/3.9.1/include/

# Exécuter l'AST Visitor (rewriter)
build/clang_ast_visitor exemples/file.c -- -I/vagrant/libs/clang_llvm/lib/clang/3.9.1/include/

# Lancez une commande depuis votre système (avec vagrant)
vagrant ssh -c "build/clang_ast_matcher exemples/<votre_fichier_a_analyser> -- -I/vagrant/libs/clang_llvm/lib/clang/3.9.1/include/"
```
