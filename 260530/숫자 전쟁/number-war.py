n = int(input())
first_cards = list(map(int, input().split()))
second_cards = list(map(int, input().split()))

# Please write your code here.
dp=[[-1]*(n+1) for _ in range(n+1)] 
#first의 i번째 카드 낼때 second의 j번째 카드 내면 얻을 수 있는 최대 점수

dp[0][0]=0

for i in range(n):
    for j in range(n):
        if dp[i][j]==-1:
            continue
        
        dp[i+1][j+1]=max(dp[i+1][j+1],dp[i][j])

        if first_cards[i]<second_cards[j]:
            dp[i+1][j]=max(dp[i+1][j],dp[i][j])
        elif second_cards[j]<first_cards[i]:
            dp[i][j+1]=max(dp[i][j+1],dp[i][j]+second_cards[j])
        else:
            dp[i+1][j+1]=max(dp[i+1][j+1],dp[i][j])


ans=0
for row in dp:
    find=row.append(ans)
    ans=max(row)
print(ans)