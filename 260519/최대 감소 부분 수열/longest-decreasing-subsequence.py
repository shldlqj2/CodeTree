n = int(input())
m = list(map(int, input().split()))

# Please write your code here.
dp=[0]*n
maxlen=0
for i in range(n):
    maxdp=0
    for j in range(i):
        if m[j]>m[i]:
            maxdp=max(dp[j],maxdp)
    dp[i]=maxdp+1
    maxlen=max(dp[i],maxlen)
print(maxlen)