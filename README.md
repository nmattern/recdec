To run first compile the file
> EX: g++ main.cpp -o main

Then run the file with one of the desired sample files or your own.
> EX: ./main sample1

Example output screenshot:

![Image of output sample](https://i.imgur.com/G2biPKD.png)

Grammar for the made up language
- [program] ::= program [block] .

- [block] ::= begin] [stmtlist] end

- [stmtlist] ::= [stmt] [morestmts]

- [morestmts] ::= ; [stmtlist] | 

- [stmt] ::= [assign] | [ifstmt] | [whilestmt] | [block]

- [assign] ::= [variable] = [expr]

- [ifstmt] ::= if [testexpr] then [stmt] else [stmt]

- [whilestmt] ::= while [testexpr] do [stmt]

- [testexpr] ::= [variable] = [expr]

- [expr] ::= + [expr] [expr] | ∗ [expr] [expr] | [variable] | [digit]

- [variable] :: = a | b | c

- [digit] :: = 0 | 1 | 2
