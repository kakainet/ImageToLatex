for var in "$@"
do
    mkdir -p "${var}_aux"
    cp transform.asy "${var}_aux/"
    mv $var "${var}_aux/"
    cd "${var}_aux"
    mv $var "raw_tex.in"
    asy transform -noV -render 5 -f png &
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
    for file in *.png
    do
        mv "$file" "sample${idx}.png"
        mv "sample${idx}.png" ../output/
        ((++idx))
    done
    cd ..
    wait
    rm -rf "${var}_aux"
done
