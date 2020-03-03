idx=0
rm -f labels.txt
for var in "$@"
do
    mkdir -p "${var}_aux"
    cp transform.asy "${var}_aux/"
    cp $var "${var}_aux/"
    cd "${var}_aux"
    mv $var "raw_tex.in"
    asy transform -noV -render 3 -f png &
    cat raw_tex.in >> ../labels.txt
    cd ..
done
mkdir -p output
wait
idx=0
for var in "$@"
do
    cd "${var}_aux"
    mv "raw_tex.in" $var
    wait
    mv $var ../
    rm -f transform.asy
    for file in `ls *.png | sort -V`;
    do 
        mv "$file" "eq${idx}.png"
        wait
        mv "eq${idx}.png" ../output/
        ((++idx)) 
    done;
    cd ..
    wait
    rm -rf "${var}_aux"
done
