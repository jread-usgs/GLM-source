#!/bin/bash

# Strip trailing blanks off source files

CWD=`pwd`
OSTYPE=`uname -s`

strip_file () {
    FILE=$1
    if [ "$OSTYPE" != "Darwin" ] ; then
       # The awk adds a trailing newline if there isn't one, macos sed does that anyway
       awk 1 $FILE | sed 's/[ \t\r]*$//' > tmpx
    else
       #  the above works for gnu-sed, but we need to replace \t with
       #  a tab character for Mac
       sed 's/[ 	]*$//' $FILE > tmpx
    fi
    \diff $FILE tmpx > /dev/null 2>&1
    if [ $? != 0 ] ; then
       echo changed $FILE
       /bin/rm $FILE
       /bin/mv tmpx $FILE
    else
       /bin/rm tmpx
    fi
}

for k in fabm-git/src/models/aed fabm-git/src/drivers/glm glm-aed glm-tests glm-egs libplot libutil libaed2 ; do
   if [ -d $k ] ; then
      cd $k
      for j in 'F90' 'c' 'h' 'm' 'sln' 'vfproj' 'vcproj' 'vcxproj' 'icproj' 'vcxproj.filters' 'nml' 'csv' 'sed' 'dat' 'sh' 'csh' 'def' 'plist' ; do
         echo "cleaning trailing spaces in $k/\*.$j"
         for i in `find . -name \*.$j` ;  do
            strip_file $i
         done
      done
      for j in .gitignore README COPYING KNOWN_ISSUES Changes Makefile changelog compat control rules;  do
         for i in `find . -name $j` ;  do
            strip_file $i
         done
      done
      cd $CWD
   else
      echo "No directory called" $k
   fi
done
for i in *.sh .gitignore README* GLM_CONFIG RELEASE-NOTES ; do
   strip_file $i
done

if [ -d fabm-git ] ; then
  tr -d '\r' < fabm-git/compilers/vs2008/fabm-glm.vfproj > .tmpx
  \diff fabm-git/compilers/vs2008/fabm-glm.vfproj .tmpx > /dev/null 2>&1
  if [ $? != 0 ] ; then
    echo changed fabm-git/compilers/vs2008/fabm-glm.vfproj
    /bin/rm fabm-git/compilers/vs2008/fabm-glm.vfproj
    /bin/mv .tmpx fabm-git/compilers/vs2008/fabm-glm.vfproj
  else
    /bin/rm .tmpx
  fi

  tr -d '\r' < fabm-git/compilers/vs2010/fabm-glm.vfproj > .tmpx
  \diff fabm-git/compilers/vs2010/fabm-glm.vfproj .tmpx > /dev/null 2>&1
  if [ $? != 0 ] ; then
    echo changed fabm-git/compilers/vs2010/fabm-glm.vfproj
    /bin/rm fabm-git/compilers/vs2010/fabm-glm.vfproj
    /bin/mv .tmpx fabm-git/compilers/vs2010/fabm-glm.vfproj
  else
    /bin/rm .tmpx
  fi
fi

