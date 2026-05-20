n = int(input())

# Please write your code here.
dp=[0]*(n+1)
dp[0]=1 #아무것도 안넣는 경우라고 가정
dp[1]=1
dp[2]=2
dp[5]=9

"""
2=dp[1](1)

3=dp[2](1,1 2) dp[1](1)
1 1 1
1 2
2 1

4= dp[3](1,1,1 1,2 2,1) dp[2](1,1 2)
1 1 1 1
2 1 1
1 2 1
1 1 2
2 2

5= dp[4](1,1,1,1 2,1,1 1,2,1 1,1,2 2,2) dp[3] (1,1,1 1,2 2,1) dp[0](None)
6= dp[5] dp[4] dp[1] = 9 + 5 + 1
"""
nums=[1,2,5]

for i in range(3,n+1):
    for j in range(3):
        if i>=nums[j]:
            if i==5:
                continue
            dp[i]+=dp[i-nums[j]]

print(dp[n]%10007)
