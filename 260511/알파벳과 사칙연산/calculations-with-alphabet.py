expression = input()

# Please write your code here.
maxResult=float('-inf')
sachik=['+','-','*','/']
subDict={}

def cac(express,idx,currResult,pmgd):
    if len(express)<=idx:
        global maxResult
        maxResult=max(currResult,maxResult)

        return

    if express[idx]=='+':
        cac(express,idx+1,currResult,'+')
    elif express[idx]=='-':
        cac(express,idx+1,currResult,'-')
    elif express[idx]=='*':
        cac(express,idx+1,currResult,'*')
    elif express[idx]=='/':
        cac(express,idx+1,currResult,'/')
    else:
        if pmgd=='+':
            if express[idx] not in subDict or subDict[express[idx]]==0:
                subDict[express[idx]]=0
                for i in range(1,5):
                    subDict[express[idx]]=i
                    cac(express,idx+1,currResult+subDict[express[idx]],pmgd)    
                subDict[express[idx]]=0
            else:
                cac(express,idx+1,currResult+subDict[express[idx]],pmgd)
            
        elif pmgd=='-':
            if express[idx] not in subDict or subDict[express[idx]]==0:
                subDict[express[idx]]=0
                for i in range(1,5):
                    subDict[express[idx]]=i
                    cac(express,idx+1,currResult-subDict[express[idx]],pmgd)    
                subDict[express[idx]]=0
            else:
                cac(express,idx+1,currResult-subDict[express[idx]],pmgd)
        elif pmgd=='*':
            if express[idx] not in subDict or subDict[express[idx]]==0:
                subDict[express[idx]]=0
                for i in range(1,5):
                    subDict[express[idx]]=i
                    cac(express,idx+1,currResult*subDict[express[idx]],pmgd)    
                subDict[express[idx]]=0
            else:
                cac(express,idx+1,currResult*subDict[express[idx]],pmgd)
        elif pmgd=='/':
            if express[idx] not in subDict or subDict[express[idx]]==0:
                subDict[express[idx]]=0
                for i in range(1,5):
                    subDict[express[idx]]=i
                    cac(express,idx+1,currResult/subDict[express[idx]],pmgd)    
                subDict[express[idx]]=0
            else:
                cac(express,idx+1,currResult/subDict[express[idx]],pmgd)

    
cac(expression,0,0,'+')
print(maxResult)
    
        
    
    
