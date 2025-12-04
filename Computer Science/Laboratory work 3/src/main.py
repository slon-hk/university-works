s = input().strip()
lst = list(s)

if 'c' in lst and 'a' in lst:
    ic = lst.index('c')
    ia = lst.index('a')
    rep = lst[ia + 1] if ia < len(lst) - 1 else 'a'
    
    if ic == 0:
        lst.insert(0, rep)
    else:
        lst[ic - 1] = rep

    print("".join(lst))