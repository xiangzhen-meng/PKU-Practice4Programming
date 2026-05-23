def f(s):
  l = len(s)
  if l <= 1:
    return s
  head = s[0]
  mid = (l+1) // 2
  lft = s[1:mid]
  rgt = s[mid:]
  return f(lft) + head + f(rgt)

inp = input()
print(f(inp))