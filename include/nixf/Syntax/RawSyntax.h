#pragma once

#include <memory>
#include <ostream>
#include <stack>
#include <vector>

namespace nixf {

class Syntax;

enum class SyntaxKind {
  // expressions, the can be evaluated to "values"
  SK_BeginExpr,
#define EXPR(NAME) SK_##NAME,
#include "SyntaxKinds.inc"
#undef EXPR
  SK_EndExpr,

  SK_BeginNode,
#define NODE(NAME) SK_##NAME,
#include "SyntaxKinds.inc"
#undef NODE
  SK_EndNode,
};

/// Abstract class for syntax nodes.
/// Currently they are Twines and Tokens.
/// Tokens has trivial aand its content
class RawNode {
protected:
  /// Text length.
  std::size_t Length;

  SyntaxKind Kind;

public:
  RawNode(SyntaxKind Kind) : Kind(Kind) {}
  /// Dump source code.
  virtual void dump(std::ostream &OS, bool DiscardTrivia = true) const = 0;

  static const char *nameOf(SyntaxKind Kind) {
    switch (Kind) {
#define EXPR(NAME)                                                             \
  case SyntaxKind::SK_##NAME:                                                  \
    return #NAME;
#define NODE EXPR
#include "SyntaxKinds.inc"
#undef EXPR
#undef NODE
    default:
      __builtin_unreachable();
    }
  }

  [[nodiscard]] const char *kindName() const { return nameOf(Kind); }

  virtual void dumpAST(std::ostream &OS, bool DiscardTrivia = true,
                       int Depth = 0) const {
    for (int I = 0; I < Depth; I++)
      OS << " ";
    OS << std::string(kindName()) << " " << Length << " ";
    if (Kind == SyntaxKind::SK_Token)
      dump(OS, DiscardTrivia);
    OS << "\n";
    std::size_t Ch = getNumChildren();
    for (std::size_t I = 0; I < Ch; I++) {
      RawNode *Ch = getNthChild(I);
      if(Ch)
        Ch->dumpAST(OS, DiscardTrivia, Depth + 1);
    }
  }

  virtual ~RawNode() = default;

  [[nodiscard]] std::size_t getLength() const { return Length; }

  [[nodiscard]] virtual std::size_t getNumChildren() const { return 0; }

  /// \returns nth child, nullptr if we cannot find such index
  [[nodiscard]] virtual RawNode *getNthChild(std::size_t N) const {
    return nullptr;
  }

  SyntaxKind getSyntaxKind() { return Kind; }
};

/// Non-term constructs in a lanugage. They have children
class RawTwine : public RawNode {
public:
  using LayoutTy = std::vector<std::unique_ptr<RawNode>>;

private:
  friend class SyntaxView;

  const std::vector<std::unique_ptr<RawNode>> Layout;

public:
  RawTwine(SyntaxKind Kind, std::vector<std::unique_ptr<RawNode>> Layout);

  void dump(std::ostream &OS, bool DiscardTrivia = true) const override;

  [[nodiscard]] std::size_t getNumChildren() const override {
    return Layout.size();
  }

  [[nodiscard]] RawNode *getNthChild(std::size_t N) const override;
};

class RawTwineBuilder {
  std::stack<std::unique_ptr<RawTwine::LayoutTy>> Stack;
  std::stack<SyntaxKind> KindStack;

public:
  void start(SyntaxKind Kind) {
    KindStack.push(Kind);
    Stack.push(std::make_unique<RawTwine::LayoutTy>());
  }
  void reset(SyntaxKind Kind) {
    KindStack.pop();
    KindStack.push(Kind);
  }
  void push(std::unique_ptr<RawNode> Node) {
    Stack.top()->emplace_back(std::move(Node));
  }
  void pop() { Stack.top()->pop_back(); }
  std::unique_ptr<RawTwine> finish() {
    RawTwine::LayoutTy Layout = std::move(*Stack.top());
    Stack.pop();
    SyntaxKind Kind = KindStack.top();
    KindStack.pop();
    auto Ret = std::make_unique<RawTwine>(Kind, std::move(Layout));
    return Ret;
  }
};

} // namespace nixf
