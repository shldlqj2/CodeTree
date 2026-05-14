n = int(input())

# Please write your code here.
"""
n=1 1
n=2 3
n=3 5
i번째까지 채울때 
i-1 까지 채운경우 1
i-2 까지 채운경우 2
dp[i]= dp[i-1]+2*dp[i-2]?
n=4 = 5+2*3 = 11
n=5 = 11+10 =21
n=6 = 21 + 22 =43
n=7 = 43 + 42 = 85
n=8 = 85+86 = 171
"""
dp=[0]*(n+1)
dp[1]=1
dp[2]=3
for i in range(3,n+1):
    dp[i]=dp[i-1]+2*dp[i-2]
print(dp[n]%10007)

