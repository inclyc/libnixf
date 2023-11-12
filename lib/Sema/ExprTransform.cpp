/// SemaView.cpp, construct Syntax nodes from raw nodes.

#include "nixf/Sema/ExprTransform.h"
#include "nixf/Basic/DiagnosticEngine.h"
#include "nixf/Basic/Range.h"
#include "nixf/Sema/Expr.h"
#include "nixf/Syntax/RawSyntax.h"
#include "nixf/Syntax/Token.h"

#include <cassert>

namespace nixf {

namespace {

LambdaExpr::Formal transformFormal(const std::shared_ptr<RawNode> &Arg,
                                   DiagnosticEngine &Diag, const char *Base) {
  assert(Arg->getSyntaxKind() == SyntaxKind::SK_Formal);
  std::string_view Name;
  std::size_t Num = Arg->getNumChildren();
  for (std::size_t I = 0; I < Num; I++) {
    std::shared_ptr<RawNode> Ch = Arg->getNthChild(I);
    switch (Ch->getSyntaxKind()) {
    case SyntaxKind::SK_Unknown:
      // ignore unknown nodes.
      break;
    case SyntaxKind::SK_Token: {
      auto Tok = std::static_pointer_cast<Token>(Ch);
      if (Tok->getKind() == tok::tok_id) {
        Name = Tok->getContent();
      }
      break;
    }
    default:
      __builtin_unreachable();
    }
  }
}

std::unique_ptr<LambdaExpr::FormalSet>
transformFormals(const std::shared_ptr<RawNode> &Arg, DiagnosticEngine &Diag,
                 const char *Base) {
  std::vector<LambdaExpr::Formal> Names;
  bool HasEllipsis = false;

  std::size_t Num = Arg->getNumChildren();
}

std::unique_ptr<LambdaExpr::FormalSet>
transformBracedFormals(const std::shared_ptr<RawNode> &Arg,
                       DiagnosticEngine &Diag) {
  std::size_t Num = Arg->getNumChildren();
  for (std::size_t I = 0; I < Num; I++) {
    std::shared_ptr<RawNode> Ch = Arg->getNthChild(I);
    switch (Ch->getSyntaxKind()) {
    case SyntaxKind::SK_Token:
    case SyntaxKind::SK_Unknown:
      // ignore unknown nodes.
      break;
    case SyntaxKind::SK_Formals: {
    }
    default:
      __builtin_unreachable();
    }
  }
  return nullptr;
}

std::pair<std::unique_ptr<LambdaExpr::FormalSet>,
          std::optional<std::string_view>>
transformLambdaArg(const std::shared_ptr<RawNode> &Arg,
                   DiagnosticEngine &Diag) {
  assert(Arg->getSyntaxKind() == SyntaxKind::SK_LambdaArg);

  std::optional<std::string_view> ArgName;
  std::unique_ptr<LambdaExpr::FormalSet> Formals = nullptr;

  std::size_t Num = Arg->getNumChildren();
  for (std::size_t I = 0; I < Num; I++) {
    std::shared_ptr<RawNode> Ch = Arg->getNthChild(I);
    switch (Ch->getSyntaxKind()) {
    case SyntaxKind::SK_Unknown:
      // ignore unknown nodes.
      break;
    case SyntaxKind::SK_Token: {
      auto Tok = std::static_pointer_cast<Token>(Ch);
      if (Tok->getKind() == tok::tok_at)
        break;
      if (Tok->getKind() == tok::tok_id) {
        ArgName = Tok->getContent();
        break;
      }
    }
    case SyntaxKind::SK_BracedFormals: {
      Formals = transformBracedFormals(Ch, Diag);
    }
    default:
      __builtin_unreachable();
    }
  }
  return {std::move(Formals), ArgName};
}

} // namespace

std::unique_ptr<Expr> visitLambda(const char *Base,
                                  const std::shared_ptr<RawNode> &Root) {
  assert(Root->getSyntaxKind() == SyntaxKind::SK_Lambda);
  std::size_t Num = Root->getNumChildren();
  for (std::size_t I = 0; I < Num; I++) {
    std::shared_ptr<RawNode> Ch = Root->getNthChild(I);
    switch (Ch->getSyntaxKind()) {
    case SyntaxKind::SK_LambdaArg:
      // check lambda arg.
      break;
    case SyntaxKind::SK_Unknown:
      // ignore unknown nodes.
      break;
    default:
      __builtin_unreachable();
    }
  }
}

} // namespace nixf
