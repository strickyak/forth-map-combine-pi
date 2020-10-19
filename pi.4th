: map  ( map-index-num -- output )
  drop rand dup *
       rand dup * +
  1 < ;

: combine ( left right -- combined )
  + ;

: map_combine ( num-maps -- combined )
  0 swap 0 do
      i map combine
  loop ;

: pi  ( num-maps -- )
  dup map_combine swap / 4 * . ;

1000000000 pi
