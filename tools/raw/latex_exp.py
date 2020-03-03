import random as r

characters = [str(el) for el in list(range(100))]

def gen_el():
    return r.choice(characters)

def gen_op():
    operators = ['+','-','\\cdot']
    return ' '+r.choice(operators)+' '

def gen_power():
    return r.choice(characters)+'^'+r.choice(characters)

def gen_frac():
    args_top = r.randint(1,2)
    args_bottom = r.randint(1,2)
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

for idx in range(250):
    for i in range(1):          
        lat=''
        wasfrac = False
        arg = r.randint(1,2)
        for j in range(arg):
            if j != 0:
                lat += gen_op()
            val = r.randint(1,100)
            if(val > 40 or wasfrac):
                lat += gen_el()
                continue
            else:
                wasfrac=True
                lat += gen_frac()
                continue
        print(lat)
            

                

