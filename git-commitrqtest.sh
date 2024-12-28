#!/bin/bash
#exit

CRTDIR=$(pwd)
echo ${CRTDIR}
cd "/storage/emulated/0/0c-msbuild/rqtest/"
#ssh-agent bash &>/dev/null &
eval `ssh-agent`
ssh-add /storage/emulated/0/git-github/id_rsa
ssh -T git@github.com
[ ! -d ./.git ] &&git init
git config --global --add safe.directory .
git lfs install
git lfs track


read -p "input the info for commit:" commitinfo

git commit -m "${commitinfo}"
#git commit -m "updata some files"
#git remote set-url origin git@github.com:x500x/gittest.git
git rm --cached -r .
git add --ignore-removal .
git remote rm origin
git remote add origin git@github.com:x500x/rqtest.git
git branch -M main
#git pull origin main --allow-unrelated-histories
git push --set-upstream origin main -f
ssh-agent -k
cd ${CRTDIR}
#bash&exit
#read -n 1
#tail -f /dev/null
