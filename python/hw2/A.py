m, n = input().split()
m, n = int(m), int(n)
a = []
for i in range(m):
  row = list(input().split())
  a.append(row)

tar = []
for j in range(n):
  row = [a[x][j] for x in range(m)]
  tar.append(row)

for i in range(n):
  print(*tar[i])