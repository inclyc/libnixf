/// Expr.h, this file decalres Expr classes that are used in Sema.
/// "Expressions" are language constructs that could be evaluated to a value.
/// Here we do some semantic checking, and transform raw AST into Expr AST.

#pragma once

#include "nixf/Basic/Range.h"

#include <memory>
#include <optional>
#include <string_view>
#include <vector>

namespace nixf {

class RawNode;

class Expr {
public:
  enum ExprKinds {
    EK_Lambda,
  };

private:
  ExprKinds Kind;

  /// The start position of this expression. This is used for error reporting.
  /// Note that this makes "Expr" impure among AST change. Semantic analysis are
  /// not designed to be "pure" or "functional".
  OffsetRange Range;

public:
  [[nodiscard]] ExprKinds getKind() const { return Kind; }

  Expr(ExprKinds Kind, OffsetRange Range) : Kind(Kind), Range(Range) {}

  [[nodiscard]] OffsetRange getRange() const { return Range; }
};

class LambdaExpr : Expr {
public:
  class Formal {
    std::string_view Name;
    OffsetRange Range;
    std::unique_ptr<Expr> Default;

  public:
    Formal(std::string_view Name, OffsetRange Range)
        : Name(Name), Range(Range) {}

    [[nodiscard]] std::string_view getName() const { return Name; }

    [[nodiscard]] OffsetRange getRange() const { return Range; }
  };

  class FormalSet {
    std::vector<Formal> Names;

    bool HasEllipsis;

  public:
    FormalSet(std::vector<Formal> Names, bool HasEllipsis)
        : Names(std::move(Names)), HasEllipsis(HasEllipsis) {}

    [[nodiscard]] const std::vector<Formal> &getNames() const { return Names; }

    [[nodiscard]] bool hasEllipsis() const { return HasEllipsis; }
  };

private:
  std::unique_ptr<FormalSet> Formals;

  std::optional<std::string_view> Arg;

  std::unique_ptr<Expr> Body;

public:
  LambdaExpr(OffsetRange Range, std::unique_ptr<class FormalSet> Formals,
             std::optional<std::string_view> Arg, std::unique_ptr<Expr> Body)
      : Expr(EK_Lambda, Range), Formals(std::move(Formals)), Arg(Arg),
        Body(std::move(Body)) {}

  [[nodiscard]] const class FormalSet *getFormals() const {
    return Formals.get();
  }

  [[nodiscard]] const std::optional<std::string_view> &getArg() const {
    return Arg;
  }

  [[nodiscard]] const Expr *getBody() const { return Body.get(); }

  static bool classof(const Expr *E) { return E->getKind() == EK_Lambda; }
};

struct AttrSet {};

} // namespace nixf
