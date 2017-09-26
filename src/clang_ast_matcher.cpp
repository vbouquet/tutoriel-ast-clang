/****************************************************************************************
*               Exemples simples d'utilisation d'AST matcher avec clang                 *
*                                                                                       *
* Author: Valentin Bouquet                                                              *
*                                                                                       *                            *
****************************************************************************************/

// Clang librairies
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/PreprocessingRecord.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PPCallbacks.h"

// System librairies
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <locale>

using namespace clang::tooling;
using namespace llvm;
using namespace std;
using namespace clang;
using namespace clang::ast_matchers;

int ERROR_PREPROCESSOR_ANALYSIS = 1;
int ERROR_AST_ANALYSIS = 2;

/***************************/
/** Liste des matchers    **/
/***************************/

// Définition/Déclaration de fonction
DeclarationMatcher FunctionDefMatcher =
    functionDecl(isExpansionInMainFile(), isDefinition()).bind("FunctionDef");

// Fonctions appelées dans le fichier
StatementMatcher FunctionCallMatcher =
    declRefExpr(isExpansionInMainFile()).bind("FunctionCall");


/******************************/
/** Définitions des matchers **/
/******************************/

class FunctionDef : public MatchFinder::MatchCallback {

public:
    virtual void run(const MatchFinder::MatchResult &Result) {
        if (const FunctionDecl *fct = Result.Nodes.getNodeAs<FunctionDecl>("FunctionDef")) {

            std::cout<<"Fonction définie: "<<fct->getNameAsString()<<std::endl;
        }
    }
};


class FunctionCall: public MatchFinder::MatchCallback {

public:
    virtual void run(const MatchFinder::MatchResult &Result) {
        if (const DeclRefExpr *fct = Result.Nodes.getNodeAs<DeclRefExpr>("FunctionCall")) {
            if ( fct->getDecl()->getKind() == Decl::Function) {
                std::cout<<"Fonction appelée: "<<fct->getFoundDecl()->getNameAsString()<<std::endl;
            }
        }
    }
};

/*
Définie les "matchers" pour les éléments du preprocesseur.
*/
class PreprocessorFinder : public PPCallbacks {

public:
    explicit inline PreprocessorFinder(const CompilerInstance &compiler)
        : compiler(compiler) {
    }

    // Récupération des inclusions de fichiers
    virtual inline void InclusionDirective(SourceLocation hashLoc,
                                           const Token &includeTok,
                                           StringRef fileName,
                                           bool isAngled,
                                           CharSourceRange filenameRange,
                                           const FileEntry *file,
                                           StringRef searchPath,
                                           StringRef relativePath,
                                           const clang::Module *imported) {

        if (compiler.getSourceManager().isInMainFile(hashLoc)) {
            std::cout<<"#include "<<fileName.str()<<std::endl;
        }
    }

private:
    const CompilerInstance &compiler;
};

// FrontEndAction pour les instructions préprocesseurs
class PreprocessorMatchingAction : public PreprocessOnlyAction {
protected:
    void ExecuteAction() {
        std::unique_ptr<PreprocessorFinder> find_includes_callback(
            new PreprocessorFinder(getCompilerInstance()));
        getCompilerInstance().getPreprocessor().addPPCallbacks(std::move(find_includes_callback));
        PreprocessOnlyAction::ExecuteAction();
    }
};

static llvm::cl::OptionCategory MyToolCategory("my-tool options");
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// Fonction utilitaire
// Ajoute un argument aux arguments passés en ligne de commande
void add_command_line_argument(int &argc, const char ** &argv, const char *arg) {
    argv[argc] = (char *) malloc(sizeof(char) * strlen(arg));
    argv[argc] = arg;
    argc++;
}

int main(int argc, const char **argv) {

    // (Optionnel) A activer pour récupérer les commentaires de types: //, ///, /* */, /** **/
    add_command_line_argument(argc, argv, "-fparse-all-comments");

    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    MatchFinder Finder;

    // Association des matchers
    FunctionDef FunctionDefFinder;
    Finder.addMatcher(FunctionDefMatcher, &FunctionDefFinder);

    FunctionCall FunctionCallFinder;
    Finder.addMatcher(FunctionCallMatcher, &FunctionCallFinder);

    // Lance l'analyse préprocesseur (pour l'analyse de toutes les directives préprocesseurs)
    int preprocessor_error = Tool.run(newFrontendActionFactory<PreprocessorMatchingAction>().get());
    if (preprocessor_error)
        return ERROR_PREPROCESSOR_ANALYSIS;

    // Suppression de la sortie superflue
    char stderr_output[50000];
    setbuf(stderr, stderr_output);

    // Lance l'analyse de l'AST avec les matchers
    int compilation_error = Tool.run(newFrontendActionFactory(&Finder).get());
    if (compilation_error)
        return ERROR_AST_ANALYSIS;

    return 0;
}