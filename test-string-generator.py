from random import choice, random, seed

seed( 10 )
alphabet = 'abcdefghijklmnopqrstuvwxyz'.upper()

letter = lambda: choice( alphabet )

with open( 'input.txt', 'w+' ) as file:
  [ file.write( f'{ letter } { int( random() * 10 ) }\n' ) for letter in alphabet ]
  [ file.write( f'{ letter1 }{ letter2 } { choice("+-/*") } { letter() } { letter() }\n' ) for letter1, letter2 in zip( alphabet, reversed( alphabet ) ) ]
  for i in range( 2200 ):
    file.write( f'{ letter() }{ letter() }{ letter() } {choice("+-/*")} { letter() }{ letter() } { letter() }{ letter() }\n' )
