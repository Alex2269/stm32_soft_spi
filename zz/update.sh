#!/bin/sh

   #******************************************
   git init
   git add .
   git commit -m "$(date "+%Y-%m-%d")"
   git remote add origin git@github.com:Alex2269/stm32_soft_spi.git
   git push -u origin master
   git pull
   #******************************************

   #******************************************
   git status
   git add .
   git commit -m "$(date "+%Y-%m-%d")"
   git push -u origin master
   git pull
   #******************************************

   #******************************************
   git checkout -b pid-new
   git branch
   git checkout master
   #******************************************
