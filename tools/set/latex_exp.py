import random as r

characters = ['0','1','2','3','4','5','6','7','8','9','x','a']

def gen_el():
    return r.choice(characters)

def gen_op():
    operators = ['+','-','\\cdot']
    return ' '+r.choice(operators)+' '

def gen_power():
    return r.choice(characters)+'^'+r.choice(characters)

def gen_frac():
    args_top = r.randint(1,3)
    args_bottom = r.randint(1,3)
    top = ''
    bottom = ''
    for i in range(args_top):
        if i != 0:
            top += gen_op()
        top += r.choice(characters)
    for i in range(args_bottom):
        if i != 0:
            bottom += gen_op()
        bottom += r.choice(characters)
    return '\\frac{'+top+'}{'+bottom+'}'

for idx in range(10):
    with open('somefile'+str(idx)+'.in', 'w') as output:
        for i in range(1000):
            lat=''
            arg = r.randint(1,3)
            for j in range(arg):
                if j != 0:
                    lat += gen_op()
                val = r.randint(1,100)
                if(val > 60):
                    lat += gen_el()
                    continue
                if(val > 30):
                    lat += gen_frac()
                    continue
                lat += gen_power()
            output.write(lat+'\n')

                

