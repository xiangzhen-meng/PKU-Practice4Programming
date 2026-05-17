m, n = (int(x) for x in input().split())
lst = list(range(m, n + 1))
lst = [x for x in lst if x % 17 == 0]
print(sum(lst))