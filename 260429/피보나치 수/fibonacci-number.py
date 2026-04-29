N = int(input())

# Please write your code here.
dp=[1]*(N+1)
dp.append(1)
dp.append(1)

def fibo(n):
    if n==1 or n==2:
        return dp[n]
    else:
        dp[n]=fibo(n-1)+fibo(n-2)
    
    return dp[n]

print(fibo(N))