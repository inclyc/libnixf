#pragma once

#include <memory>
#include <vector>

namespace nixf {

enum class SyntaxKind { Token };

class SyntaxView;

class RawSyntax {
  friend class SyntaxView;
  const SyntaxKind Kind;

  const std::vector<std::shared_ptr<RawSyntax>> Layout;

public:
  RawSyntax(SyntaxKind Kind, std::vector<std::shared_ptr<RawSyntax>> Layout)
      : Kind(Kind), Layout(std::move(Layout)) {}
};

} // namespace nixf
