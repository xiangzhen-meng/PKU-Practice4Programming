inp = input().split()
x, y, op = int(inp[0]), int(inp[1]), inp[2]
if op == '+':
  print(x + y)
elif op == '-':
  print(x - y)
elif op == '*':
  print(x * y)
elif op == '/':
  if y != 0:
    print(x // y)
  else:
    print('Divided by zero!')
else:
  print('Invalid operator!')