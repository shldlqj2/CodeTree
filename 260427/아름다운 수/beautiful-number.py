n = int(input())

# Please write your code here.

subarr=[]

result=0

def makenum(cnt,samecnt):
    global result


    if cnt==n:
        last=subarr[-1]
        if samecnt==1:
            if last==1:
                result+=1
        else:
            if last==1:
                result+=1
            elif last==2:
                if samecnt%2==0:
                    result+=1
                else:
                    return
            elif last==3:
                if samecnt%3==0:
                    result+=1
                else:
                    return
            elif last==4:
                if samecnt%4==0:
                    result+=1
                else:
                    return
        return
    if cnt==0:
        for i in range(1,5):
            subarr.append(i)
            makenum(cnt+1,samecnt+1)
            subarr.pop()
    else:
        bef=subarr[-1]
        for i in range(1,5):
            if bef==i:
                subarr.append(i)
                makenum(cnt+1,samecnt+1)
                subarr.pop()
            else:
                
                if bef==1:
                    subarr.append(i)
                    makenum(cnt+1,1)
                    subarr.pop()
                elif bef==2:
                    if samecnt%2==0:
                        subarr.append(i)
                        makenum(cnt+1,1)
                        subarr.pop()

                elif bef==3:
                    if samecnt%3==0:
                        subarr.append(i)
                        makenum(cnt+1,1)
                        subarr.pop()

                elif bef==4:
                    if samecnt%4==0:
                        subarr.append(i)
                        makenum(cnt+1,1)
                        subarr.pop()

                        
        
        

makenum(0,0)
print(result)