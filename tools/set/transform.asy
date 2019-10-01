file fin = input("raw_tex.in").line();
int idx = 1;

while (!eof(fin))
{
    string s = fin;
    label("$\displaystyle " + s + "$", (0,0));
    shipout("eq"+string(idx));
    erase();
    ++idx;
}