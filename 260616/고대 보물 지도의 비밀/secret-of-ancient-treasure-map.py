n, k = map(int, input().split())
numbers = list(map(int, input().split()))

# Please write your code here.
dp=[[float('-inf')]*(k+1) for _ in range(n+1)] #i번째 선택핳때 음수j개



for j in range(k+1):
    for i in range(n):
        if numbers[i]<0:
            if j==k:
                continue
            else:
                if dp[i][j]==float('-inf'):
                    dp[i+1][j+1]=max(dp[i+1][j+1],numbers[i])
                else:
                    dp[i+1][j+1]=max(dp[i+1][j+1],dp[i][j]+numbers[i])
        else:
            if dp[i][j]==float('-inf'):
                dp[i+1][j]=max(dp[i+1][j],numbers[i])
            else:
                dp[i+1][j]=max(dp[i+1][j],dp[i][j]+numbers[i])
mn=float('-inf')

for i in range(n+1):
    for j in range(k+1):
        mn=max(mn,dp[i][j])
print(mn)

    
