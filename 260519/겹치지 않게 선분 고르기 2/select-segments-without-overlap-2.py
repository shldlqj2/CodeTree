n = int(input())
x1, x2 = [], []
for _ in range(n):
    a, b = map(int, input().split())
    x1.append(a)
    x2.append(b)

# Please write your code here.
points=list(zip(x1,x2))
points.sort(key=lambda x:(
    x[0],
    x[1]))

dp=[1]*n



for i in range(n):
    for j in range(i):
        if points[j][0]<=points[i][0]<=points[j][1] or \
        points[j][0]<=points[i][1]<=points[j][1] or \
        points[i][0]<=points[j][0]<=points[i][1] or \
        points[i][0]<=points[j][1]<=points[i][1]:
            continue
        dp[i]=max(dp[i],dp[j]+1)

print(max(dp))