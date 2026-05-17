n = input()
nums = input().split()
cnt = 0
for num in nums:
  if int(num[0]) + int(num[1]) + int(num[2]) < int(num[3]):
    cnt += 1
print(cnt)