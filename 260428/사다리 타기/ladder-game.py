n, m = map(int, input().split())
edges = [tuple(map(int, input().split())) for _ in range(m)]

# Please write your code here.
edges.sort(key=lambda x: x[1])
curredges=[]


def routingStart(edgemap):
    result=[]
    for i in range(1,n+1):
        curr=i
        for a,b in edgemap:
            if curr==a:
                curr+=1
            elif curr==a+1:
                curr-=1
        result.append(curr)
    return result

def findMinimum(linecnt):
    if linecnt==m:
        return

    currresult=routingStart(curredges)

    cnt=0
    for i in range(len(currresult)):
        if currresult[i]!=defaultResult[i]:
            break
        else:
            cnt+=1
    if cnt == n:
        global result
        result=min(result,linecnt)
        return        

    
    

    for i in range(linecnt,m):
        for j in range(1,n):
            curredges.append((j,i+1))
            findMinimum(linecnt+1)
            curredges.pop()
        

defaultResult=routingStart(edges)

result=float('inf')

findMinimum(0)
print(result)