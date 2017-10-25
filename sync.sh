#!/bin/bash

sudo git add .

read -p "Enter: " menssage

git commit -m $menssage

git push -f origin master
