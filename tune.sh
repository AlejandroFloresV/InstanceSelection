#!/bin/bash

# PBIL
for init in Random ClosestEnemy FarthestEnemy
do
  for f in iris iris2d wdbc segmentation banana
  do
    for iter in 100 1000 10000
    do
      for pop in 20 50 80
      do
        for mp in 001 005 01 02
        do
          for ms in 005 01
          do
            for lr in 005 01 02
            do
              for nlr in 005 075 01
              do
                ./full -alg PBIL -init $init -f data/$f.norm -iter $iter -pop $pop -mp 0.$mp -ms 0.$ms -lr 0.$lr -nlr 0.$nlr > trash.txt
				echo "./full -alg PBIL -init $init -f data/$f.norm -iter $iter -pop $pop -mp 0.$mp -ms 0.$ms -lr 0.$lr -nlr 0.$nlr" >> tunePBIL.txt
				bin/average < output.txt >> tunePBIL.txt
              done
            done
          done
        done
      done
    done
  done
done

rm trash.txt
