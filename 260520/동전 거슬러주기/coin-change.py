N, M = map(int, input().split())
coin = list(map(int, input().split()))

# Please write your code here.
dp=[float('inf')]*(M+1)
dp[0]=0

for i in range(1,M+1):
    for num in coin:
        if i-num >= 0:
            dp[i]=min(dp[i],dp[i-num]+1)

if dp[M]==float('inf'):
    print(-1)
else:
    print(dp[M])