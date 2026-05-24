n = int(input())
profit = list(map(int, input().split()))

# Please write your code here.
dp=[float('-inf')]*(n+1) #길이 최대 수익
dp[0]=0

for i in range(1,n+1):
    for j in range(n):
        if i-j-1 >= 0:
            dp[i]=max(dp[i],dp[i-j-1]+profit[j])

print(dp[-1])