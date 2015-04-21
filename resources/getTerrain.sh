#!/bin/bash

wget http://opengameart.org/sites/default/files/terrain_0.zip
unzip terrain_0.zip
rm terrain_0.zip
cd terrain
mogrify -monitor -format bmp *.jpg
