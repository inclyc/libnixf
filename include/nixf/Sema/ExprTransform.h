/// SemaView.cpp, construct Syntax nodes from raw nodes.

#include "Expr.h"

namespace nixf {

/// Construct "Expr" AST from raw nodes.
std::unique_ptr<Expr> transformExpr(const std::shared_ptr<RawNode> &Root,
                                    const char *Base);

} // namespace nixf
