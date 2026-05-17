n, m = map(int, input().split())
dishes = {}

for _ in range(m):
    name, price, amount = input().split()
    dishes[name] = [int(price), int(amount)]

total = 0
for _ in range(n):
    for name in input().split():
        price, amount = dishes[name]
        if amount > 0:
            total += price
            dishes[name][1] -= 1

print(total)