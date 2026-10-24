n = int(input())

# Please write your code here.
dp=[[0]*10 for _ in range(1001)]

for i in range(1,10):
    dp[1][i]=1

for i in range(2,1001):
    for j in range(10):
        if j==0:
            if i==2:
                dp[i][j]=1
            else:
                dp[i][j]=dp[i-1][j+1]%(10**9 + 7)
        elif j==9:
            dp[i][j]=dp[i-1][j-1]%(10**9 + 7)
        else:
            dp[i][j]=(dp[i-1][j-1]+dp[i-1][j+1])%(10**9 + 7)
# print(dp[n])
print(sum(dp[n])%(10**9 + 7))
