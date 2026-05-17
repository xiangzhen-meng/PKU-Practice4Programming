N, M = (int(x) for x in input().split())
lights = [1] * N

for m in range(1, M + 1):
  for i in range(m - 1, N, m):
    lights[i] *= -1

print(','.join(str(i + 1) for i in range(N) if lights[i] == -1))