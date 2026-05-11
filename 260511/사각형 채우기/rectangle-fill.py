n = int(input())

# Please write your code here.
"""
n==1 -> 1
n==2 -> 2
n==3 -> 3
n==4 -> 
"""
arr=[0]*1001
arr[1]=1
arr[2]=2

for i in range(3,n+1):
    arr[i]=arr[i-1]+arr[i-2]

print(arr[n]%10007)