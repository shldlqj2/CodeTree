N = int(input())

# Please write your code here.
from collections import deque

arr=[0]*(1000001)


queue=deque()
queue.append(N)

while queue:
    currnum=queue.popleft()
    currtime=arr[currnum]

    cnd3=currnum//3
    cnd2=currnum//2

    if currnum==1:
        print(currtime)
        break

    if 1<=currnum-1<1000001 and arr[currnum-1]==0:
        arr[currnum-1]=currtime+1
        queue.append(currnum-1)
    
    if 1<=currnum+1<1000001 and arr[currnum+1]==0:
        arr[currnum+1]=currtime+1
        queue.append(currnum+1)
    
    if currnum%2==0 and 1<=cnd2<1000001 and arr[cnd2]==0:
        arr[cnd2]=currtime+1
        queue.append(cnd2)

    if currnum%3==0 and 1<=cnd3<1000001  and arr[cnd3]==0:
        arr[cnd3]=currtime+1
        queue.append(cnd3)
