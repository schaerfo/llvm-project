//===--- UseBracedInitializationCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UseBracedInitializationCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::modernize {

void UseBracedInitializationCheck::registerMatchers(MatchFinder *Finder) {
  // auch möglicherweise interessant: hasSyntacticForm, isListInitialization,
  // cxxStdInitializerListExpr
  Finder->addMatcher(
      traverse(TK_IgnoreUnlessSpelledInSource,
               varDecl(unless(anyOf(
                   parmVarDecl(), has(cxxConstructExpr(argumentCountIs(0)))))))
          .bind("var"),
      this);
  Finder->addMatcher(traverse(TK_IgnoreUnlessSpelledInSource,
                              cxxCtorInitializer(unless(
                                  anyOf(withInitializer(initListExpr()),
                                        withInitializer(cxxConstructExpr())))))
                         .bind("ctor"),
                     this);
  Finder->addMatcher(
      fieldDecl(hasInClassInitializer(unless(initListExpr()))).bind("field"),
      this);
  Finder->addMatcher(
      traverse(TK_IgnoreUnlessSpelledInSource,
               cxxConstructExpr(
                   unless(anyOf(isListInitialization(), hasParent(varDecl()))))
                   .bind("construct")),
      this);
}

void UseBracedInitializationCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedVarDecl = Result.Nodes.getNodeAs<VarDecl>("var");
  const auto *MatchedCtorInitializer =
      Result.Nodes.getNodeAs<CXXCtorInitializer>("ctor");
  const auto *Field = Result.Nodes.getNodeAs<FieldDecl>("field");
  const auto *Construct = Result.Nodes.getNodeAs<CXXConstructExpr>("construct");

  if (MatchedVarDecl) {
    checkVarDecl(MatchedVarDecl);
  }

  if (MatchedCtorInitializer) {
    checkCtorInitializer(MatchedCtorInitializer);
  }

  if (Field) {
    checkField(Field);
  }

  if (Construct) {
    checkConstruct(Construct);
  }
}

void UseBracedInitializationCheck::checkCtorInitializer(
    const CXXCtorInitializer *Initializer) {
  diag(Initializer->getSourceLocation(),
       "Use braced initialization in constructor initializer");
}

void UseBracedInitializationCheck::checkVarDecl(const VarDecl *Var) {
  if (Var->getInitStyle() == VarDecl::InitializationStyle::CallInit)
    diag(Var->getLocation(),
         "Use braced initialization in variable initialization");
}

void UseBracedInitializationCheck::checkField(const FieldDecl *Field) {
  diag(Field->getLocation(), "Use braced initialization in field declaration");
}

void UseBracedInitializationCheck::checkConstruct(
    const CXXConstructExpr *Construct) {
  diag(Construct->getLocation(),
       "Use braced initialization in object construction");
}

} // namespace clang::tidy::modernize
