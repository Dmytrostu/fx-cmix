a=open("modified_enwik9", 'r', encoding='utf-8').read()
from collections import Counter
from tqdm import tqdm
def get_symbols():
    t=["e","t","o","n","i","h","s","r","d","l","u","m",",","w","c","f","g","y","p","b","'","v","k","-","M",'"',";","S","!","H","C","x","W","D","?"]
    a=t[:]
    for l0 in a:
        for l1 in a:
            t.append(l0+l1)
    return t
t=get_symbols()

words=a.split(" ")
word_set=set(words)
c=Counter(words).most_common()
d={}
for i, s in enumerate(t):
    d[c[i][0]]=s

r=[]
y=set(d.values())
for i,word in tqdm(enumerate(words),total=len(words)):
    if word in d:
        r.append(" "+d[word])
    else:
        if word in y:
            r.append(" "+chr(0)+word)
        else:
            if i==0:
                r.append(word)
            else:
                r.append(" "+word)

x="\n".join(d.keys())+"#######"+"".join(r)
b=open("a", 'w', encoding='utf-8')
b.write(x)
b.close()
