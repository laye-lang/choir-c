// Macro Expansion Tests
//
// NOTE: Throughout this file, empty expansions should be on a
// line of their own. Our pretty printer isn’t clever enough to
// e.g. realise that 'b' in 'a b' should be printed on a separate
// line if 'a' expands to nothing.
//

#define A
A
A A
A A A A A A
#undef A

// + 123
// + 123 123 123 123
#define A 123
A
A A A A
#undef A

// + A A
A A

// + A
// + A)
// + +
#define A(a) a + a
A
A)
A()

// + 1 + 1
// + 2 + 2 3 + 3
A(1)
A(2) A(3)
#undef A

// + a b ab 1234 ++ -= ==
#define A(x, y) x##y
A(,)
A(a,) A(,b) A(a,b) A(12,34) A(+, +) A(-,=) A(==,) A(,==) A(  =  ,    =)
#undef A

// + m(1, 2 ) m(1, 2, 1) m(1, 2, 1, 2, 3)
#define A(...) m(1, 2 __VA_OPT__(,) __VA_ARGS__)
A() A(1) A(1, 2, 3)
#undef A

// + X X
// + X 1 1 X
// + X 2 2 X
// + X X
// + X X
// + X 3 X
// + X 12 12 X
// + X 12 12 X
// + X 12 12 X
// + X 12 132 X
#define A(a,b,c,...) X a##__VA_OPT__()##b a##__VA_OPT__(c)##b X
A(,,,)
A(1,,)
A(,2,,)
A(,,3,)
A(,,,4)
A(,,3,4)
A(1,2,,)
A(1,2,3,)
A(1,2,3)
A(1,2,3,4)


// + "X X"
// + "X 1 1 X"
// + "X 2 2 X"
// + "X X"
// + "X X"
// + "X 3 X"
// + "X 12 12 X"
// + "X 12 12 X"
// + "X 12 12 X"
// + "X 12 132 X"
#define S2(x) #x
#define S(x) S2(x)
S(A(,,,))
S(A(1,,))
S(A(,2,,))
S(A(,,3,))
S(A(,,,4))
S(A(,,3,4))
S(A(1,2,,))
S(A(1,2,3,))
S(A(1,2,3))
S(A(1,2,3,4))

// + "A(,,,)"
// + "A(1,,)"
// + "A(,2,,)"
// + "A(,,3,)"
// + "A(,,,4)"
// + "A(,,3,4)"
// + "A(1,2,,)"
// + "A(1,2,3,)"
// + "A(1,2,3)"
// + "A(1,2,3,4)"
S2(A(,,,))
S2(A(1,,))
S2(A(,2,,))
S2(A(,,3,))
S2(A(,,,4))
S2(A(,,3,4))
S2(A(1,2,,))
S2(A(1,2,3,))
S2(A(1,2,3))
S2(A(1,2,3,4))
#undef A
#undef S
#undef S2

// + (1, 2)
// + (1, 2)
// + (1, 42)
// + (1, 42)
#define A(x, y) (x, y)
#define VaFirst(x, ...) x
#define Default42(x, ...) A(x, VaFirst(__VA_ARGS__ __VA_OPT__(,) 42))
Default42(1, 2)
Default42(1, 2, 3)
Default42(1,)
Default42(1)
#undef A
#undef VaFirst
#undef Default42

// + 1 + 1
// + 1 + 1
// + 1 + 1
#define L (
#define R )
#define Id(x) x
#define Q(x) x + x
#define E()
Id(Q L 1 R)
Id(Q Id(L) 1 Id(R))
Id(Q Id(E)() Id(L) 1 Id(R))
#undef L
#undef R
#undef Id
#undef Q
#undef E

// + f(2 * (f(2 * (z[0]))));
#define f(a) f(x * (a))
#define x 2
#define z z[0]
f(f(z));
#undef f
#undef x
#undef z

// + A B
#define A B
#define B A
A B
#undef A
#undef B

// + A B C A B A C A B C A
#define A A B C
#define B B C A
#define C C A B
A
#undef A
#undef B
#undef C

// + int i(void)
#define i(x) h(x
#define h(x) x(void)
extern int i(i));
#undef i
#undef h

// + a: 2 + M_0(3)(4)(5);
// + b: 4 + 4 + 3 + 2 + 1 + M_0(3)(2)(1);
#define M_0(x) M_ ## x
#define M_1(x) x + M_0(0)
#define M_2(x) x + M_1(1)
#define M_3(x) x + M_2(2)
#define M_4(x) x + M_3(3)
#define M_5(x) x + M_4(4)
a: M_0(1)(2)(3)(4)(5);
b: M_0(5)(4)(3)(2)(1);
#undef M_0
#undef M_1
#undef M_2
#undef M_3
#undef M_4
#undef M_5

// + c: m a X
#define n(v) v
#define l m
#define m l a
c: n(m) X
#undef n
#undef l
#undef m

// + A: Y
#define X() Y
#define Y() X
A: X()()()
#undef X
#undef Y

// + B: f()
// + C: for()
#define f(x) h(x
#define for(x) h(x
#define h(x) x()
B: f(f))
C: for(for))
#undef f
#undef for
#undef h

#define IDENTITY1(x) x
#define IDENTITY2(x) IDENTITY1(x) IDENTITY1(x) IDENTITY1(x) IDENTITY1(x)
#define IDENTITY3(x) IDENTITY2(x) IDENTITY2(x) IDENTITY2(x) IDENTITY2(x)
#define IDENTITY4(x) IDENTITY3(x) IDENTITY3(x) IDENTITY3(x) IDENTITY3(x)
#define IDENTITY5(x) IDENTITY4(x) IDENTITY4(x) IDENTITY4(x) IDENTITY4(x)
#define IDENTITY6(x) IDENTITY5(x) IDENTITY5(x) IDENTITY5(x) IDENTITY5(x)
#define IDENTITY7(x) IDENTITY6(x) IDENTITY6(x) IDENTITY6(x) IDENTITY6(x)
#define IDENTITY8(x) IDENTITY7(x) IDENTITY7(x) IDENTITY7(x) IDENTITY7(x)
#define IDENTITY9(x) IDENTITY8(x) IDENTITY8(x) IDENTITY8(x) IDENTITY8(x)
#define IDENTITY0(x) IDENTITY9(x) IDENTITY9(x) IDENTITY9(x) IDENTITY9(x)
IDENTITY0()

// + first second third
#define FOO() BAR() second
#define BAR()
first FOO() third
#undef FOO
#undef BAR

// + bar foo (2)
#define foo(x) bar x
foo(foo) (2)
#undef foo

// + m(ABCD)
#define m(a) a(w)
#define w ABCD
m(m)
#undef m
#undef w

// + FUNC (3+1)
#define F(a) a
#define FUNC(a) (a+1)
F(FUNC) FUNC (3);
#undef F
#undef FUNC

// + # define X 3
#define H #
#define D define
#define DEFINE(a, b) H D a b
DEFINE(X, 3)
#undef H
#undef D
#undef DEFINE

// + a:Y
#define FOO(X) X ## Y
a:FOO()
#undef FOO

// + b:Y
#define FOO2(X) Y ## X
b:FOO2()
#undef FOO2

// + c:YY
#define FOO3(X) X ## Y ## X ## Y ## X ## X
c:FOO3()
#undef FOO3

// + d:FOO4(,)
#define FOO4(X, Y) X ## Y
d:FOO4(,FOO4(,))
#undef FOO4

// + AB AB CD
#define CD A ## B C ## D
#define AB A ## B C ## D
AB
#undef AB
#undef CD

// + 1: aaab 2
#define a(n) aaa ## n
#define b 2
1: a(b b)
#undef a
#undef b

// + 2: 2 baaa
#define a(n) n ## aaa
#define b 2
2: a(b b)

// + 3: 2 xx
#define baaa xx
3: a(b b)
#undef baaa
#undef a
#undef b

// + "x ## y";
#define hash_hash # ## #
#define mkstr(a) # a
#define in_between(a) mkstr(a)
#define join(c, d) in_between(c hash_hash d)
join(x, y);
#undef hash_hash
#undef mkstr
#undef in_between
#undef join

// + A ## B;
#define FOO(x) A x B
FOO(##);
#undef FOO

// + !!
#define A(B,C) B ## C
!A(,)!
#undef A

// + A: barbaz123
#define FOO bar ## baz ## 123
A: FOO
#undef FOO

// + B: ##
#define M1(A) A
#define M2(X) X
B: M1(M2(##))
#undef M1
#undef M2

// + int ei_1 = (17+1);
// + int ei_2 = (M1)(17);
#define M1(a) (a+1)
#define M2(b) b
int ei_1 = M2(M1)(17);
int ei_2 = (M2(M1))(17);
#undef M1
#undef M2

// + a: 2*f(9)
#define f(a) a*g
#define g f
a: f(2)(9)
#undef f
#undef g

// + b: 2*9*g
#define f(a) a*g
#define g(a) f(a)
b: f(2)(9)
#undef f
#undef g

#define LPAREN (
#define RPAREN )
#define F(x, y) x + y
#define ELLIP_FUNC(...) __VA_ARGS__

// + 1: F, (, 'a', 'b', );
// + 2: 'a' + 'b';
1: ELLIP_FUNC(F, LPAREN, 'a', 'b', RPAREN);
2: ELLIP_FUNC(F LPAREN 'a', 'b' RPAREN);
#undef F
#undef ELLIP_FUNC

// + 3 ;
#define i(x) 3
#define a i(yz
#define b )
a b ) ;
#undef i
#undef a
#undef b

// + static int glob = (1 + 1 );
#define FUNC(a) a
static int glob = (1 + FUNC(1 RPAREN );
#undef FUNC

#define A0 expandedA0
#define A1 expandedA1 A0
#define A2 expandedA2 A1
#define A3 expandedA3 A2

#define A() B LPAREN )
#define B() C LPAREN )
#define C() D LPAREN )

// + 1: F, (, 'a', 'b', );
// + 2: 'a' + 'b';
#define F(x, y) x + y
#define ELLIP_FUNC(...) __VA_OPT__(__VA_ARGS__)
1: ELLIP_FUNC(F, LPAREN, 'a', 'b', RPAREN);
2: ELLIP_FUNC(F LPAREN 'a', 'b' RPAREN);
#undef F
#undef ELLIP_FUNC

// + 3: f(0 , a, b, c)
// + 4: f(0 )
#define F(...) f(0 __VA_OPT__(,) __VA_ARGS__)
3: F(a, b, c)
4: F()
#undef F

// + 5: f(0, a , b, c)
// + 6: f(0, a )
// + 7: f(0, a )
// + 7.1: f(0, a , ,)
#define G(X, ...) f(0, X __VA_OPT__(,) __VA_ARGS__)
5: G(a, b, c)
6: G(a)
7: G(a,)
7.1: G(a,,)
#undef G

// + 8: HT_
// + 9: TONG C ( ) B ( ) "A()"
#define HT_B() TONG
#define F(x, ...) HT_ ## __VA_OPT__(x x A()  #x)
8: F(1)
9: F(A(),1)
#undef HT_B
#undef F

// + 10: ""
// + 11: "A1 expandedA1 expandedA0 B ( )"
#define F(a,...) #__VA_OPT__(A1 a)
10: F(A())
11: F(A1 A(), 1)
#undef F

// + 12.0:
// + 12:
// + 13: BB
#define F(a,...) a ## __VA_OPT__(A1 a) ## __VA_ARGS__ ## a
12.0: F()
12: F(,)
13: F(B,)
#undef F

// + 14: "" X ""
// + 15: "" X ""
#define F(...) #__VA_OPT__()  X ## __VA_OPT__()  #__VA_OPT__(        )
14: F()
15: F(1)
#undef F

// + 16: S foo ;
// + 17: S bar = { 1, 2 };
#define SDEF(sname, ...) S sname __VA_OPT__(= { __VA_ARGS__ })
16: SDEF(foo);
17: SDEF(bar, 1, 2);
#undef SDEF

// + 18: B ( ) "" B ( )
// + 19: B ( ) "" B ( )
// + 20: B ( ) "A3 expandedA3 expandedA2 expandedA1 expandedA0 A3C A3" B ( )
// + 21: B ( ) "A3 B ( ),expandedA0 A3A(),A0A3C A3" B ( )
#define F(a,...) A() #__VA_OPT__(A3 __VA_ARGS__ a ## __VA_ARGS__ ## a ## C A3) A()
18: F()
19: F(,)
20: F(,A3)
21: F(A3, A(),A0)
#undef F

// + 22: B ( ) "" B ( )
// + 23: B ( ) "" B ( )
// + 24: B ( ) "A3 expandedA0 A0C A3" expandedA0 expandedA0 A0C expandedA0 B ( )
// + 25: B ( ) "A3 B ( ),expandedA0 A0A(),A0A0C A3" expandedA0 expandedA0 C ( ),expandedA0 A0A(),A0A0C expandedA0 B ( )
#define F(a,...) A() #__VA_OPT__(A3 __VA_ARGS__ a ## __VA_ARGS__ ## a ## C A3) a __VA_OPT__(A0 __VA_ARGS__ a ## __VA_ARGS__ ## a ## C A0) A()
22: F()
23: F(,)
24: F(,A0)
25: F(A0, A(),A0)
#undef F

// + 26: B 1
// + 26_1: B 1
#define F(a,...)  __VA_OPT__(B a ## a) ## 1
#define G(a,...)  __VA_OPT__(B a) ## 1
26: F(,1)
26_1: G(,1)
#undef F
#undef G

// + 27: B 11
// + 27_1: BexpandedA0 11
// + 28: B 11
#define F(a,...)  B ## __VA_OPT__(a 1) ## 1
#define G(a,...)  B ## __VA_OPT__(a ## a 1) ## 1
27: F(,1)
27_1: F(A0,1)
28: G(,1)
#undef F
#undef G

#undef A0
#undef A1
#undef A2
#undef A3
#undef A
#undef B
#undef C

// + 1: int x = 42;
#define LPRN() (
#define G(Q) 42
#define F1(R, X, ...)  __VA_OPT__(G R X) )
1: int x = F1(LPRN(), 0, <:-);
#undef LPRN
#undef G
#undef F1

// + 2: f(0 )
#define F2(...) f(0 __VA_OPT__(,) __VA_ARGS__)
#define EMP
2: F2(EMP)
#undef F2
#undef EMP

// + 3: ""
#define H3(X, ...) #__VA_OPT__(X##X X##X)
3: H3(, 0)
#undef H3

// + 4: a b
#define H4(X, ...) __VA_OPT__(a X ## X) ## b
4: H4(, 1)
#undef H4

// + 4: a b
#define H4B(X, ...) a ## __VA_OPT__(X ## X b)
4: H4B(, 1)
#undef H4B

// + 5: ab
#define H5A(...) __VA_OPT__()/**/__VA_OPT__()
#define H5B(X) a ## X ## b
#define H5C(X) H5B(X)
5: H5C(H5A())
#undef H5A
#undef H5B
#undef H5C

// + 6: ab
#define H6(X, ...) __VA_OPT__(a ## X) ## b
6: H6(, 1);
#undef H6
