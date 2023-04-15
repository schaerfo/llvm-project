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
  // auch möglicherweise interessant: hasSyntacticForm, isListInitialization, cxxStdInitializerListExpr
  Finder->addMatcher(traverse(TK_IgnoreUnlessSpelledInSource, varDecl(unless(parmVarDecl()))).bind("var"), this);
  Finder->addMatcher(traverse(TK_IgnoreUnlessSpelledInSource, cxxCtorInitializer(unless(withInitializer(initListExpr())))).bind("ctor"), this);
  Finder->addMatcher(fieldDecl(hasInClassInitializer(unless(initListExpr()))).bind("field"), this);
}

void UseBracedInitializationCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedVarDecl = Result.Nodes.getNodeAs<VarDecl>("var");
  const auto *MatchedCtorInitializer = Result.Nodes.getNodeAs<CXXCtorInitializer>("ctor");
  const auto *Field = Result.Nodes.getNodeAs<FieldDecl>("field");

  if (MatchedVarDecl) {
    checkVarDecl(MatchedVarDecl);
  }

  if (MatchedCtorInitializer) {
    checkCtorInitializer(MatchedCtorInitializer);
  }

  if (Field) {
    checkField(Field);
  }
}

void UseBracedInitializationCheck::checkCtorInitializer(
    const CXXCtorInitializer *Initializer) {
  diag(Initializer->getSourceLocation(), "Use braced initialization in constructor initializer");
}

void UseBracedInitializationCheck::checkVarDecl(const VarDecl *Var) {
  if (Var->getInitStyle() == VarDecl::InitializationStyle::CallInit)
    diag(Var->getLocation(), "Use braced initialization in variable initialization");
}

void UseBracedInitializationCheck::checkField(const FieldDecl *Field) {
  diag(Field->getLocation(), "Use braced initialization in field declaration");
}

} // namespace clang::tidy::modernize
