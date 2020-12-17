VDecl<0> Identifier<pi> Assg<0> Number<3.14159> StEnd<0> 
FDecl<0> Identifier<giveYouUp> LPA<0> RPA<0> BStart<0> 
Identifier<drawCircle> LPA<0> Number<20> Comma<0> Number<20> Comma<0> Number<6> Comma<0> Number<43> RPA<0> StEnd<0> 
Identifier<drawCircle> LPA<0> Number<20> Comma<0> Number<20> Comma<0> Number<2> Comma<0> Number<42> RPA<0> StEnd<0> 
Identifier<drawCircle> LPA<0> Number<40> Comma<0> Number<10> Comma<0> Number<3> RPA<0> StEnd<0> 
Identifier<drawCircle> LPA<0> Number<30> Comma<0> Number<10> Comma<0> Number<8> Comma<0> Number<42> RPA<0> StEnd<0> 
Identifier<drawCircle> LPA<0> Number<50> Comma<0> Number<10> Comma<0> Number<2> RPA<0> StEnd<0> 
Identifier<drawCircle> LPA<0> Number<50> Comma<0> Number<15> Comma<0> Number<7> Comma<0> Number<45> RPA<0> StEnd<0> 
BEnd<0> 
FDecl<0> Identifier<drawCircle> LPA<0> Identifier<x0> Comma<0> Identifier<y0> Comma<0> Identifier<r> RPA<0> BStart<0> 
Identifier<drawCircle> LPA<0> Identifier<x0> Comma<0> Identifier<y0> Comma<0> Identifier<r> Comma<0> Number<39> RPA<0> StEnd<0> 
BEnd<0> 
FDecl<0> Identifier<drawCircle> LPA<0> Identifier<x0> Comma<0> Identifier<y0> Comma<0> Identifier<r> Comma<0> Identifier<symbol> RPA<0> BStart<0> 
VDecl<0> Identifier<angle> Assg<0> Number<0> StEnd<0> 
VDecl<0> Identifier<step> Assg<0> Identifier<pi> Div<0> Number<100> StEnd<0> 
While<0> LPA<0> Identifier<angle> Le<0> Identifier<pi> Div<0> Number<2> RPA<0> BStart<0> 
VDecl<0> Identifier<x> Assg<0> Sin<0> LPA<0> Identifier<angle> RPA<0> Mul<0> Number<2> Mul<0> Identifier<r> StEnd<0> 
VDecl<0> Identifier<y> Assg<0> Cos<0> LPA<0> Identifier<angle> RPA<0> Mul<0> Identifier<r> StEnd<0> 
Setpix<0> LPA<0> Identifier<x0> Plus<0> Identifier<x> RPA<0> LPA<0> Identifier<y0> Plus<0> Identifier<y> RPA<0> Identifier<symbol> StEnd<0> 
Setpix<0> LPA<0> Identifier<x0> Minus<0> Identifier<x> RPA<0> LPA<0> Identifier<y0> Plus<0> Identifier<y> RPA<0> Identifier<symbol> StEnd<0> 
Setpix<0> LPA<0> Identifier<x0> Minus<0> Identifier<x> RPA<0> LPA<0> Identifier<y0> Minus<0> Identifier<y> RPA<0> Identifier<symbol> StEnd<0> 
Setpix<0> LPA<0> Identifier<x0> Plus<0> Identifier<x> RPA<0> LPA<0> Identifier<y0> Minus<0> Identifier<y> RPA<0> Identifier<symbol> StEnd<0> 
Identifier<angle> AdAssg<0> Identifier<step> StEnd<0> 
BEnd<0> 
BEnd<0> 
FDecl<0> Identifier<dre> LPA<0> Identifier<x0> Comma<0> Identifier<y0> Comma<0> Identifier<r> Comma<0> Identifier<symbol> RPA<0> BStart<0> 
VDecl<0> Identifier<angle> Assg<0> Number<0> StEnd<0> 
VDecl<0> Identifier<step> Assg<0> Identifier<pi> Div<0> Number<100> StEnd<0> 
While<0> LPA<0> Identifier<angle> Le<0> Identifier<pi> Div<0> Number<2> RPA<0> BStart<0> 
VDecl<0> Identifier<x> Assg<0> Sin<0> LPA<0> Identifier<angle> RPA<0> Mul<0> Number<2> Mul<0> Identifier<r> StEnd<0> 
VDecl<0> Identifier<y> Assg<0> Cos<0> LPA<0> Identifier<angle> RPA<0> Mul<0> Identifier<r> StEnd<0> 
Setpix<0> LPA<0> Identifier<x0> Plus<0> Identifier<x> RPA<0> LPA<0> Identifier<y0> Plus<0> Identifier<y> RPA<0> Identifier<symbol> StEnd<0> 
Setpix<0> LPA<0> Identifier<x0> Minus<0> Identifier<x> RPA<0> LPA<0> Identifier<y0> Plus<0> Identifier<y> RPA<0> Identifier<symbol> StEnd<0> 
Setpix<0> LPA<0> Identifier<x0> Minus<0> Identifier<x> RPA<0> LPA<0> Identifier<y0> Minus<0> Identifier<y> RPA<0> Identifier<symbol> StEnd<0> 
Setpix<0> LPA<0> Identifier<x0> Plus<0> Identifier<x> RPA<0> LPA<0> Identifier<y0> Minus<0> Identifier<y> RPA<0> Identifier<symbol> StEnd<0> 
Identifier<angle> AdAssg<0> Identifier<step> StEnd<0> 
BEnd<0> 
BEnd<0> 
None<0> 