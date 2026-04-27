N = int(input())

# Please write your code here.
from collections import deque

arr=[0]*(N+1)


queue=deque()
queue.append(N)

while queue:
    currnum=queue.popleft()
    currtime=arr[currnum]


    if currnum==1:
        break

    if 1<=currnum-1<N and arr[currnum-1]==0:
        arr[currnum-1]=currtime+1
        queue.append(currnum-1)
    
    if 1<=currnum+1<N and arr[currnum+1]==0:
        arr[currnum+1]=currtime+1
        queue.append(currnum+1)
    
    if currnum%2==0 and 1<=currnum//2<N and arr[currnum//2]==0:
        arr[currnum//2]=currtime+1
        queue.append(currnum//2)

    if currnum%3==0 and 1<=currnum//3<N  and arr[currnum//3]==0:
        arr[currnum//3]=currtime+1
        queue.append(currnum//3)

print(arr[1])