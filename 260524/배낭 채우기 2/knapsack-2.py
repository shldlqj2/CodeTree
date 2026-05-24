N, M = map(int, input().split())
w, v = zip(*[tuple(map(int, input().split())) for _ in range(N)])
w, v = list(w), list(v)

# Please write your code here.
dp=[float('-inf')]*(M+1)
dp[0]=0
for i in range(M+1):
    for j in range(N):
        if i-w[j]>=0:
            dp[i]=max(dp[i],dp[i-w[j]]+v[j])
print(max(dp))