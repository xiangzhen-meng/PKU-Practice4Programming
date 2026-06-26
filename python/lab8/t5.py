def p2oct(p, x):
  ans = 0
  w = 1
  for xi in reversed(x):
    if xi <= 'Z' and xi >= 'A':
      xi = ord(xi) - ord('A') + 10
    else:
      xi = int(xi)
    ans += int(xi) * w
    w *= p
  return ans

def oct2q(q, x):
  ans = ''
  while x > 0:
    if x % q > 9:
      ans += chr(ord('A') + x % q - 10)
    else:
      ans += str(x % q)
    x //= q
  return ans[::-1]

m = int(input())
for _ in range(m):
  p, x, q = input().split(',')
  p, q = int(p), int(q)
  if x == '0':
    print(0)
    continue
  x = p2oct(p, x)
  print(oct2q(q, x))