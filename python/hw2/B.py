def cmpkind(x):
  if x[0] == 'h':
    return 0
  elif x[0] == 's':
    return 1
  elif x[0] == 'd':
    return 2
  else:
    return 3
  
def cmp(x):
  if x[1:] == '2':
    return 13
  elif x[1:] == 'A':
    return 12
  elif x[1:] == 'K':
    return 11
  elif x[1:] == 'Q':
    return 10
  elif x[1:] == 'J':
    return 9
  else:
    return int(x[1:]) - 2

try:
  while True:
    a = list(input().split())
    a.sort(key=cmpkind)
    a.sort(key=cmp, reverse=True)
    print(*a)
except EOFError:
  pass