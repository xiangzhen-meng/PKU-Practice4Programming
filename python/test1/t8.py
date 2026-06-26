n = int(input())
mp = set()
for _ in range(n):
  place = input().split('-')
  cur = place[0]
  mp.add(cur)
  for nxt in place[1:]:
    cur += '-' + nxt
    mp.add(cur)
  
q = int(input())
for _ in range(q):
  place = input()
  if place in mp:
    print('Yes')
  else:
    print('No')
    