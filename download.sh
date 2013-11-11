#!/bin/bash

# bash download.sh [resolution: 2km | 1km | 500m | 250m] [year, eg: 2012] [day_start: 0-365] [day_end: 0-365] [path: r14c20] [output_dir: data]

res=$1
year=$2
day_start=$3
day_end=$4
path=$5

output_dir=$6

if [ -z "$1" ]; then
  res="1km"
fi

if [ -z "$2" ]; then
  year="2013"
fi

if [ -z "$3" ]; then
  day_start=100
  day_end=130
fi

if [ -z "$5" ]; then
  path="r14c20"
fi

if [ -z "$6" ]; then
  output_dir="./data"
fi

mkdir -p $output_dir
cd $output_dir
for day in $(eval echo $year"{$day_start..$day_end}"); do
  path_file="http://lance-modis.eosdis.nasa.gov/imagery/subsets/RRGlobal_$path/$day/RRGlobal_$path.$day.terra.$res.jpg";
  echo $path_file;

  curl -O $path_file;
done
cd ..