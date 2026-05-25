n = int(input())
coin = [0] + list(map(int, input().split()))

# Please write your code here.
# 1번 오른 경우 최대 3

dp=[[float('-inf')]*4 for _ in range(n+1)] #currFloor, oneJump

dp[0][0]=0


for i in range(n):
    for j in range(4):
        if i<n-1:
            dp[i+2][j]=max(dp[i][j]+coin[i+2],dp[i+2][j])
        if j==3:
            continue
        dp[i+1][j+1]=max(dp[i][j]+coin[i+1],dp[i+1][j+1])
        

print(max(dp[n]))