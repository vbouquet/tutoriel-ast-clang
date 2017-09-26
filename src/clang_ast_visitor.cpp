//------------------------------------------------------------------------------
// Clang rewriter sample. Demonstrates:
//
// * How to use RecursiveASTVisitor to find interesting AST nodes.
// * How to use the Rewriter API to rewrite the source code.
//
// Eli Bendersky (eliben@gmail.com)
// This code is in the public domain
//------------------------------------------------------------------------------
#include <cstdio>
#include <memory>
#include <string>
#include <sstream>

#include <iostream>
#include <fstream>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
public:
    MyASTVisitor(Rewriter &R, std::string funcName, std::string suffix) :
        TheRewriter(R), funcName(funcName), suffix(suffix) {}

    // Déclaration de tous les prototypes visiteur
    bool VisitFunctionDecl(FunctionDecl *f);

private:
  Rewriter &TheRewriter;
    std::string funcName;
    std::string suffix;
};



/**
 * Implémentation d'un visiteur, parcourant les noeuds de type FunctionDecl
**/
bool MyASTVisitor::VisitFunctionDecl(FunctionDecl *f) {
    DeclarationNameInfo DeclNameInfo = f->getNameInfo();
    DeclarationName DeclName = DeclNameInfo.getName();
    std::string fName = DeclName.getAsString();

    // Si le nom de la fonction ne correspond à celui recherché alors on
    // passe au prochain noeud.
    if (fName != funcName)
        return true;

    // Si la fonction à un corps (fonction définie) alors on récupère la position du
    // nom de la fonction et y ajoutte un suffix.
    if (f->hasBody()) {
        SourceLocation ST = DeclNameInfo.getLocStart().getLocWithOffset(fName.length());
        TheRewriter.InsertTextAfter(ST, suffix);
    }

    return true;
}


// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MyASTConsumer : public ASTConsumer {
public:
    MyASTConsumer(Rewriter &R, std::string funcName, std::string suffix) :
        Visitor(R, funcName, suffix) {}

  // Override the method that gets called for each parsed top-level
  // declaration.
  virtual bool HandleTopLevelDecl(DeclGroupRef DR) {
    for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b)
      // Traverse the declaration using our AST visitor.
      Visitor.TraverseDecl(*b);
    return true;
  }

private:
  MyASTVisitor Visitor;
};

int main(int argc, char *argv[]) {

    std::string outputFolder = "output/";
    std::string filePath = std::string(argv[1]);

    std::string funcName = "somme";
    std::string suffix   = "_mglwnafh_Cthulhu_Rlyeh_wgah_nagl_fhtagn";

    CompilerInstance TheCompInst;
    TheCompInst.createDiagnostics();

    LangOptions &lo = TheCompInst.getLangOpts();
    lo.C99 = 1;

    // Initialize target info with the default triple for our platform.
    auto TO = std::make_shared<TargetOptions>();
    TO->Triple = llvm::sys::getDefaultTargetTriple();
    TargetInfo *TI =
        TargetInfo::CreateTargetInfo(TheCompInst.getDiagnostics(), TO);
    TheCompInst.setTarget(TI);

    TheCompInst.createFileManager();
    FileManager &FileMgr = TheCompInst.getFileManager();
    TheCompInst.createSourceManager(FileMgr);
    SourceManager &SourceMgr = TheCompInst.getSourceManager();
    TheCompInst.createPreprocessor(TU_Module);
    TheCompInst.createASTContext();

    // A Rewriter helps us manage the code rewriting task.
    Rewriter TheRewriter;
    TheRewriter.setSourceMgr(SourceMgr, TheCompInst.getLangOpts());

    // Set the main file handled by the source manager to the input file.
    const FileEntry *FileIn = FileMgr.getFile(filePath);
    SourceMgr.setMainFileID(
        SourceMgr.createFileID(FileIn, SourceLocation(), SrcMgr::C_User));
    TheCompInst.getDiagnosticClient().BeginSourceFile(
        TheCompInst.getLangOpts(), &TheCompInst.getPreprocessor());

    // Create an AST consumer instance which is going to get called by
    // ParseAST.
    MyASTConsumer TheConsumer(TheRewriter, funcName, suffix);

    // Parse the file to AST, registering our consumer as the AST consumer.
    ParseAST(TheCompInst.getPreprocessor(), &TheConsumer,
             TheCompInst.getASTContext());

    // At this point the rewriter's buffer should be full with the rewritten
    // file contents.
    const RewriteBuffer *RewriteBuf =
        TheRewriter.getRewriteBufferFor(SourceMgr.getMainFileID());

    // Contenu du fichier après réécriture (sous forme de chaîne de caractères),
    // le fichier original n'est pas modifié.
    std::ofstream f;
    f.open(outputFolder + "file.c");
    f<<std::string(RewriteBuf->begin(), RewriteBuf->end());
    f.close();

    return 0;
}
