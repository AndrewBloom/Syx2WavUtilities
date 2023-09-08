#!/bin/bash

################################################################################
# Bash Script Name: [syx2wavScript]
# Description: [This script travels through all the subdirectories of the input 
#               directory, and converts all syx sample files found to wav, using
#               sndfile-convert (must be present on system), and syx2standard.out
#               the path of which is specified on the variable STRIPPER.
#               syx2standard.out is an exe file, the source code of which should
#               be in the same repo of this file.]
# Author: [Andrew Bloom]
# License: [MIT License]
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# For more details, please refer to the standard MIT LICENSE description online.
################################################################################
set -e

echo "Directory is : "${1}

TARGET_DIR=${1}

STRIPPER="/Users/bloom/sw_develop/BloomApps/Audio/SampleConverter/syx2standard.out"

cd ${TARGET_DIR}

ls | while read dir 
do 
	mkdir -p wav/${dir}
	cd ${dir}
	ls *syx | while read file
	do
		${STRIPPER} ${file} ../wav/${dir}/${file}
	done
	cd ../wav/${dir}
	ls | while read file
	do
		FILENAME=$(basename -- "${file}" ".syx")
		sndfile-convert -pcm16 ${file} ${FILENAME}.wav || echo "is it a sound file or not? (kits are not soundfiles." # this line could file, "or" bypasses set -e
		rm ${FILENAME}.syx # better be sure we can mess only with syx files and not delete something else
	done
	cd ../..
done
