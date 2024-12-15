from random import choice, random, seed

seed( 10 )
alphabet = 'abcdefghijklmnopqstuvwxyz'.upper()

letter = lambda: choice( alphabet )

dual = [ f'{ letter1 }{ letter2 }' for letter1, letter2 in zip( alphabet, reversed( alphabet ) ) ]
triple = [ f'{ letter() }{ letter() }{ letter() }' for _ in range(5200) ] 
quadruple = [ f'{ letter() }{ letter() }{ letter() }{ letter() }' for _ in range(20530) ]

with open( 'input-s.txt', 'w+' ) as file:
  [ file.write( f'{ letter } { int( random() * 10 ) }\n' ) for letter in alphabet ]
  [ file.write( f'{ duo } { choice("+-/*") } { letter() } { letter() }\n' ) for duo in dual ]
  file.write( f'R { choice( dual ) }' ) 

with open( 'input-l.txt', 'w+' ) as file:
  [ file.write( f'{ letter } { int( random() * 10 ) }\n' ) for letter in alphabet ]
  [ file.write( f'{ duo } { choice("+-/*") } { letter() } { letter() }\n' ) for duo in dual ]
  [ file.write( f'{ trio } { choice("+-/*") } { choice(dual) } { choice(dual) }\n' ) for trio in triple]
  file.write( f'R { choice(triple) }' ) 

with open( 'input-xl.txt', 'w+' ) as file:
  [ file.write( f'{ letter } { int( random() * 10 ) }\n' ) for letter in alphabet ]
  [ file.write( f'{ duo } { choice("+-/*") } { letter() } { letter() }\n' ) for duo in dual ]
  [ file.write( f'{ trio } { choice("+-/*") } { choice(dual) } { choice(dual) }\n' ) for trio in triple]
  [ file.write( f'{ quad } { choice("+-/*") } { choice(triple) } { choice(triple) }\n' ) for quad in quadruple]
  file.write( f'R { choice(quadruple) }' )
