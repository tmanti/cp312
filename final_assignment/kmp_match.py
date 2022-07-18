def create_lps(p):
    p_len = len(p) #get length of the pattern

    lps = [0]*p_len #initialize longest prefix that is suffix array

    m = 0 #set first iterator variable
    n = 1 #set second iterator variable
    while(n < p_len): #while upper bound is less than length of pattern
        if(p[m] == p[n]): #if they are the same
            m+=1 #incrememnt lower bond
            lps[n] = m #set prefix suffix to new level
            n+=1 # increment upper bound
        elif(m == 0): # if lower bound is first character
            lps[n] = 0 #set prefix suffix to 0
            n+=1 # increment upper bound
        else: #if lower bound not the same
            m = lps[m-1] #go to previous prefix suffix of last character

    return lps # return built array


def kmp(text, p):
    lps = create_lps(p) #create lps array from pattern
    print(lps)

    t_len = len(text) #length of text
    p_len = len(p) #length of pattern
    
    text_curr = 0 # current point in iteration of given text
    p_curr = 0 # current point in pattern

    while(text_curr < t_len):
        #print(text[text_curr], p[p_curr])
        if(text[text_curr] == p[p_curr]): # if matching point in pattern
            if(p_curr == p_len-1):#check if pattern found within text string
                print("Matching substring found at: " + str(text_curr+1-p_len))
                p_curr = lps[p_curr-1] #move to prefix suffix point of last character in lps array (possible pattern intersection)
            text_curr += 1 #look at next char in text
            p_curr += 1 #look at next char in pattern
        elif(p_curr > 0):
            p_curr = lps[p_curr]
        else: # if pattern is at beginning
            text_curr += 1 # incrememnt text position

kmp("ABABDABACDABABCABABCABAB", "ABABCABAB")