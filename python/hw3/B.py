def deepcopy(a):
  b = []
  for i in a:
    if isinstance(i, list) or isinstance(i, tuple):
      b.append(deepcopy(i))
    else:
      b.append(i)
  if isinstance(a, list):
    return b
  else:
    return tuple(b)
a = [1,2,[3,[4],5],(6,[7,[8],9])]
b = deepcopy(a)
print(b)
a[2][1].append(400)
a[3][1][1].append(800)
print(a)
print(b)