#!/bin/bash

# Source mods -- simplified ## dxui a15 qpr2

echo "Applying source mods... please wait!"

cd ../../../hardware/interfaces/compatibility_matrices

rm compatibility_matrix.7.xml
rm compatibility_matrix.6.xml
rm compatibility_matrix.5.xml

wget https://raw.githubusercontent.com/SuperAviation001/sourcemods/refs/heads/15_v2/compatibility_matrix.5.xml
wget https://raw.githubusercontent.com/SuperAviation001/sourcemods/refs/heads/15_v2/compatibility_matrix.6.xml
wget https://raw.githubusercontent.com/SuperAviation001/sourcemods/refs/heads/15_v2/compatibility_matrix.7.xml

cd ../../../device/nothing/tetris

# Clone other trees

while true; do
    read -p "Would you like to clone the other trees? (vendor and kernel). Choose yes(y) or no(n) " yn
    case $yn in
        [Yy]* ) $PWD/clone-others.sh; break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no.";;
    esac
done
