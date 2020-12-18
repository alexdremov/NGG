
FDecl<0> Identifier<one_sol> LPA<0> Identifier<sol> RPA<0> BStart<0> 
Print<0> Number<1> StEnd<0> 
Print<0> Identifier<sol> StEnd<0> 
BEnd<0> 
FDecl<0> Identifier<two_sol> LPA<0> Identifier<sol1> Comma<0> Identifier<sol2> RPA<0> BStart<0> 
Print<0> Number<2> StEnd<0> 
Print<0> Identifier<sol1> StEnd<0> 
Print<0> Identifier<sol2> StEnd<0> 
BEnd<0> 
FDecl<0> Identifier<no_sol> LPA<0> RPA<0> BStart<0> 
Print<0> Number<0> StEnd<0> 
BEnd<0> 
FDecl<0> Identifier<inf_sol> LPA<0> RPA<0> BStart<0> 
Print<0> Minus<0> Number<1> Mul<0> Number<1> StEnd<0> 
BEnd<0> 
FDecl<0> Identifier<giveYouUp> LPA<0> RPA<0> BStart<0> 
VDecl<0> Identifier<a> Assg<0> Input<0> StEnd<0> 
VDecl<0> Identifier<b> Assg<0> Input<0> StEnd<0> 
VDecl<0> Identifier<c> Assg<0> Input<0> StEnd<0> 
If<0> LPA<0> Identifier<a> Eq<0> Number<0> RPA<0> BStart<0> 
If<0> LPA<0> Identifier<b> Eq<0> Number<0> RPA<0> BStart<0> 
If<0> LPA<0> Identifier<c> Eq<0> Number<0> RPA<0> BStart<0> 
Return<0> Identifier<inf_sol> LPA<0> RPA<0> StEnd<0> 
BEnd<0> Else<0> BStart<0> 
Return<0> Identifier<no_sol> LPA<0> RPA<0> StEnd<0> 
BEnd<0> 
BEnd<0> Else<0> BStart<0> 
Return<0> Identifier<one_sol> LPA<0> LPA<0> Minus<0> Number<1> Mul<0> Identifier<c> RPA<0> Div<0> Identifier<b> RPA<0> StEnd<0> 
BEnd<0> 
BEnd<0> 
VDecl<0> Identifier<diskr> Assg<0> LPA<0> Identifier<b> Mul<0> Identifier<b> RPA<0> Minus<0> LPA<0> Number<4> Mul<0> LPA<0> Identifier<a> Mul<0> Identifier<c> RPA<0> RPA<0> StEnd<0> 
If<0> LPA<0> Identifier<diskr> Gr<0> Number<0> RPA<0> BStart<0> 
Identifier<diskr> Assg<0> Identifier<diskr> Pow<0> Number<2> StEnd<0> 
Return<0> Identifier<two_sol> LPA<0> LPA<0> Identifier<diskr> Minus<0> Identifier<b> RPA<0> Div<0> LPA<0> Number<2> Mul<0> Identifier<a> RPA<0> Comma<0> LPA<0> LPA<0> Minus<0> Number<1> Mul<0> Identifier<diskr> RPA<0> Minus<0> Identifier<b> RPA<0> Div<0> LPA<0> Number<2> Mul<0> Identifier<a> RPA<0> RPA<0> StEnd<0> 
BEnd<0> Else<0> BStart<0> 
If<0> LPA<0> Identifier<diskr> Eq<0> Number<0> RPA<0> BStart<0> 
Return<0> Identifier<one_sol> LPA<0> LPA<0> Minus<0> Number<1> Mul<0> Identifier<b> RPA<0> Div<0> LPA<0> Number<2> Mul<0> Identifier<a> RPA<0> RPA<0> StEnd<0> 
BEnd<0> Else<0> BStart<0> 
Return<0> Identifier<no_sol> LPA<0> RPA<0> StEnd<0> 
BEnd<0> 
BEnd<0> 
BEnd<0> 
None<0> 