This example shows that:

- if file1.c has "int x" (uninitialized and so weak) and file2.c has "int x=56" (initialized
	and therefore strong) symbols, they will merge in the link phase.
	This means the two files will share the x symbol.

- if two files do not concur about the type of a symbol, and one of them has the symbol as
	weak (possibly both) then all hell can break loose.
