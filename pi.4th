: map  ( i -- o )
  drop rand dup *
       rand dup * +
  1 < ;

: combine ( l r -- c )
  + ;

: map_combine ( n -- c )
  0 swap 0 do
      i map combine
  loop ;

: pi
  dup map_combine swap / 4 * . ;

100000000 pi
