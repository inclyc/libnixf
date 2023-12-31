/// SyntaxKinds.inc, declares syntax names.

/// provides: EXPR(Name)  - expressions. They can be evaluated.
/// provides: NODE(Name)  - simple node. They cannot be evaluated.

// Expressions.
#ifdef EXPR
EXPR(Assert)
EXPR(AttrSet)
EXPR(Call)
EXPR(Float)
EXPR(Int)
EXPR(If)
EXPR(Lambda)
EXPR(Let)
EXPR(List)
EXPR(OpNegate)
EXPR(OpNot)
EXPR(OpHasAttr)
EXPR(OpAnd)
EXPR(OpBinary)
EXPR(Path)
EXPR(Select)
EXPR(String)
EXPR(IndString)
EXPR(Var)
EXPR(With)
EXPR(Paren)
// let { ..., .body }, desugared to rec { ..., .body }.body
EXPR(LegacyLet)
#endif // EXPR

#ifdef NODE
NODE(Token)
NODE(TriviaPiece)
NODE(Trivia)
NODE(ID)
NODE(Formals)
NODE(Formal)
NODE(IndStringParts)
NODE(StringParts)
NODE(Interpolation)
NODE(AttrPath)
NODE(Inherit)
NODE(Binds)
NODE(Binding)
NODE(ListBody)
NODE(LambdaArg)
NODE(BracedFormals)
NODE(Unknown)
NODE(Root)
NODE(EOF)
#endif // NODE
