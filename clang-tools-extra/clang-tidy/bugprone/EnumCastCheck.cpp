//===--- EnumCastCheck.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "EnumCastCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bugprone {

void EnumCastCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(explicitCastExpr(hasDestinationType(enumType())).bind("x"), this);
}

void EnumCastCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<ExplicitCastExpr>("x");
  auto SourceType = MatchedDecl->getSubExpr()->getType();
  if (SourceType->isBuiltinType()) {
    diag(MatchedDecl->getBeginLoc(),
         "You are casting an int to an enum");
  }

  if (isa<EnumType>(SourceType) && SourceType.getTypePtr() != MatchedDecl->getTypeAsWritten().getCanonicalType().getTypePtr()) {
    diag(MatchedDecl->getBeginLoc(), "You are casting an enum to a different enum");
  }
}

} // namespace bugprone
} // namespace tidy
} // namespace clang
