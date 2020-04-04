file fin = input("raw_tex.in").line();
int idx = 0;

while (!eof(fin))
{
    string s = fin;
    label("$\displaystyle " + s + "$", (0,0));
    shipout("e"+string(idx));
    erase();
    ++idx;
}
