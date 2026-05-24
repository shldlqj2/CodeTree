N, M = map(int, input().split())
w, v = zip(*[tuple(map(int, input().split())) for _ in range(N)])
w, v = list(w), list(v)

# Please write your code here.
dp=[float('-inf')]*(M+1) #dp에는 i번째 무게 최댓가치
dp[0]=0 #해당 무게에서 최대 가치
for i in range(N): #1번씩만 선택
    for j in range(M,0,-1):
        if j>=w[i]:
            if dp[j-w[i]]==float('inf'):
                continue
            dp[j]=max(dp[j],dp[j-w[i]]+v[i])
print(max(dp))