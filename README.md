# A simple, purely-concatenative, pointless FORTH. 

The only data type on the stack is double floating point.

Written in C++ with only standard libraries.

(Written for experimenting with map-combine (like map-reduce)
and a simple monte-carlo algorithm to find Pi --
but map-combine is not here yet.)

## Usage

Compile the `forth` command by typing `make`.

The last argument of `forth` is a string of words (separated by white space) to be executed.
Use quotes around the words so the shell will see it as one final argument.

```
make
./forth '100 23 + .'
123
```

If you want to define new words, alternate name and definition arguments.
There is no ':' command or any other defining word.

```
./forth  plus '+'  squared 'dup *'  '3 squared 4 squared plus .'
25
```

## Summary

The only data type on the stack is double-precision C++ floating point numbers.
All arithmetic, relational, and logical operations operate on doubles and return doubles.

In tests, 0.0 is false, and other numbers are true.

Relational operators return 1.0 for true and 0.0 for false.

#### Operators

`+ - * / %` --  Binary operators, as in C.  Each takes two numbers from
the stack and puts one number back on.

`< <= > >= = <>` --  Relational operators. Each takes two numbers from
the stack and puts one number (which will be 1.0 for true, or 0.0
for false) back on.

`dup swap rot drop` --  The usual Forth combinators.

`.`  -- print a number from the stack, plus a space.

`\`whatever`  -- Any word starting with the back-quote `\`` is printed,
without the initial back-quote, when the word is executed, plus a space.

Literal number composed of the range `[-0-9.]` push themselves
onto the stack when executed.

#### Do Loops

Use `do` and `loop`.  Only counts up by 1s, and uses `<` as the critereon to continue looping.
You can use `i` to get the current index variable, and `j` for one outer level.
There is no `leave`.

#### While Loops (like BEGIN WHILE REPEAT).

Use the word `while:` followed by two words.
Call them P and B, for predicate and body.
It first calls P, which should leave a boolean value on top of the stack.
If that value is true, it calls B, and then repeats, calling P again.
But if the value returned by P is false, it exits the loop.

If you need more than one word as part of P or B (and you probably do),
define new words with what you want, and use the new word for P or B.

#### Conditional Execution (like IF ELSE THEN)

Use the word `cond:` followed by two words.
Call them Y and N, for yes and no.
First `cond:` takes one value from the stack.
If that value is true, it calls Y.
Otherwise it calls N.

If you need more than one word as part of Y or N (and you probably do),
define new words with what you want, and use the new word for Y or N.

#### Tests

The tests in the file `test.cc` demonstrate how to use these
looping and conditional operators.
