n = int(input())

if n % 4 == 0 and not (n % 100 == 0 and n % 400 != 0)\
  and n % 3200 != 0:
    print('Y')
else:
  print('N')