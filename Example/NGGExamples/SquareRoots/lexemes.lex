FDecl<0> Identifier<giveYouUp> LPA<0> RPA<0> BStart<0> 
Identifier<solveSquareEquation> LPA<0> Input<0> Comma<0> Input<0> Comma<0> Input<0> RPA<0> StEnd<0> 
BEnd<0> 
FDecl<0> Identifier<solveSquareEquation> LPA<0> Identifier<a> Comma<0> Identifier<b> Comma<0> Identifier<c> RPA<0> BStart<0> 
If<0> LPA<0> Identifier<a> Eq<0> Number<0> RPA<0> BStart<0> 
If<0> LPA<0> Identifier<b> Eq<0> Number<0> RPA<0> BStart<0> 
If<0> LPA<0> Identifier<c> Eq<0> Number<0> RPA<0> BStart<0> 
Print<0> Number<1> Div<0> Number<0> StEnd<0> 
BEnd<0> Else<0> BStart<0> 
Print<0> Number<0> StEnd<0> 
BEnd<0> 
BEnd<0> Else<0> BStart<0> 
Print<0> Number<1> StEnd<0> 
Print<0> Minus<0> Identifier<c> Div<0> Identifier<b> StEnd<0> 
BEnd<0> 
BEnd<0> Else<0> BStart<0> 
VDecl<0> Identifier<d> Assg<0> Identifier<b> Pow<0> Number<2> Minus<0> Number<4> Mul<0> Identifier<a> Mul<0> Identifier<c> StEnd<0> 
If<0> LPA<0> Number<0> Eq<0> Identifier<d> RPA<0> BStart<0> 
Print<0> Number<1> StEnd<0> 
Print<0> Minus<0> Identifier<b> Div<0> LPA<0> Number<2> Mul<0> Identifier<a> RPA<0> StEnd<0> 
BEnd<0> Else<0> BStart<0> 
If<0> LPA<0> Identifier<d> Gr<0> Number<0> RPA<0> BStart<0> 
Print<0> Number<2> StEnd<0> 
Print<0> LPA<0> Minus<0> Identifier<b> Plus<0> Identifier<d> Pow<0> Number<0.5> RPA<0> Div<0> LPA<0> Number<2> Mul<0> Identifier<a> RPA<0> StEnd<0> 
Print<0> LPA<0> Minus<0> Identifier<b> Minus<0> Identifier<d> Pow<0> Number<0.5> RPA<0> Div<0> LPA<0> Number<2> Mul<0> Identifier<a> RPA<0> StEnd<0> 
BEnd<0> Else<0> BStart<0> 
Print<0> Number<0> StEnd<0> 
BEnd<0> 
BEnd<0> 
BEnd<0> 
BEnd<0> 
None<0> 