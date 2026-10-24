import sys
input = sys.stdin.readline

N, M = map(int, input().split())
clothes = [tuple(map(int, input().split())) for _ in range(N)]
s = [x[0] for x in clothes]
e = [x[1] for x in clothes]
v = [x[2] for x in clothes]

# Please write your code here.
beforeCloth = [[-float('inf')] * N for _ in range(M)]
dp = [[0]*N for _ in range(M)]



for i in range(M):
    for j in range(N):
        if s[j]-1 <= i and i <= e[j]-1 :
            beforeCloth[i][j]=v[j]

# print(beforeCloth)

for i in range(1,M):
    for j in range(N):
        for k in range(N):
            if j != k and beforeCloth[i][j] != -float('inf') and beforeCloth[i-1][k] != -float('inf'):
                dp[i][j]= max(dp[i][j],dp[i-1][k] + abs(beforeCloth[i][j]-beforeCloth[i-1][k]))
            elif j==k:
                dp[i][j]=max(dp[i][j],dp[i-1][j])

# print(dp)
print(max(dp[M-1]))
