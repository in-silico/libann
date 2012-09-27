#! /bin/bash

outdir="xytest"
rm -r $outdir
mkdir $outdir

for d in $(ls ./pic); do
    mkdir "$outdir/$d"
    for f in $(ls ./pic/$d); do
	p="$d/$f"
	./xyapply.sh "pic/$p" "$outdir/$p"
    done
done
