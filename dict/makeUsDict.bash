#!/bin/bash

#https://vaneyckt.io/posts/safer_bash_scripts_with_set_euxo_pipefail/
set -euo pipefail

#show debug info
#set -x

#move to script dir
cd "$(dirname "${0}")"

#save root dir
ROOT=$(pwd)

function CheckNeeded {
  NEEDED=$(which ${1})
  if [ ! -f "${NEEDED}" ]; then
    echo "You need ${1} to run this script!"
    exit 0
  fi
}
function changeLetter {
    sed -i "s/${1}/${2}/" ${filename}.1
}
function fixLetterFile {
  #remove - (create backup file)
  filename=${1}
  sed -r '/.*-.*/d' ${filename} > ${filename}.1

  #remove . (inplace)
  sed -i -r '/.*\..*/d' ${filename}.1

  #remove ! (inplace)
  sed -i -r '/.*!.*/d' ${filename}.1

  #remove / (inplace)
  sed -i -r '/.*\/.*/d' ${filename}.1

  #remove ′ (inplace)
  sed -i -r '/.*′.*/d' ${filename}.1

  #remove ` (inplace)
  sed -i -r '/.*`.*/d' ${filename}.1

  #remove ʿ (inplace)
  sed -i -r '/.*ʿ.*/d' ${filename}.1

  #remove @ (inplace)
  sed -i -r '/.*@.*/d' ${filename}.1

  #remove synonim message (inplace)
  sed -i -r 's/\  \\| .*$//' ${filename}.1

  #remove comma (inplace)
  sed -i -r '/.*,.*/d' ${filename}.1

  #remove '
  sed -i -r "/.*'.*/d" ${filename}.1

  #remove +
  sed -i -r "/.*\+.*/d" ${filename}.1

  #remove a̅
  sed -i -r "/.*a̅.*/d" ${filename}.1

  #remove ʾ
  sed -i -r "/.*ʾ.*/d" ${filename}.1

  #remove numbers (inplace)
  sed -i -r '/.*0.*/d' ${filename}.1
  sed -i -r '/.*1.*/d' ${filename}.1
  sed -i -r '/.*2.*/d' ${filename}.1
  sed -i -r '/.*3.*/d' ${filename}.1
  sed -i -r '/.*4.*/d' ${filename}.1
  sed -i -r '/.*5.*/d' ${filename}.1
  sed -i -r '/.*6.*/d' ${filename}.1
  sed -i -r '/.*7.*/d' ${filename}.1
  sed -i -r '/.*8.*/d' ${filename}.1
  sed -i -r '/.*9.*/d' ${filename}.1

  #change letters
  #if the name uses those signals, then comment all lines and find especific words...
  changeLetter ç c
  changeLetter á a
  changeLetter é e
  changeLetter í i
  changeLetter ó o
  changeLetter ú u
  changeLetter à a
  changeLetter è e
  changeLetter ì i
  changeLetter ò o
  changeLetter ù u
  changeLetter ã a
  changeLetter õ o
  changeLetter â a
  changeLetter ê e
  changeLetter ô o
  changeLetter ä a
  changeLetter ë e
  changeLetter ï i
  changeLetter ö o
  changeLetter ü u
  changeLetter ñ n
  changeLetter ā a
  changeLetter ō o
  changeLetter ø o
  changeLetter ø o
  changeLetter ū u
  changeLetter á a
  changeLetter ğ g
  changeLetter å a
  changeLetter ş s
  changeLetter ǎ a
  changeLetter ŏ o
  changeLetter û u
  changeLetter î i
  changeLetter ř r
  changeLetter ö o
  changeLetter ő o
  changeLetter ć c
  changeLetter ī i
  changeLetter ś s
  changeLetter š s
  changeLetter é e
  changeLetter ł l
  changeLetter ł l
  changeLetter ţ t
  changeLetter ǔ u
  changeLetter æ a
  changeLetter ę e
  changeLetter é e

  #add more substitutions here...

  #all unique
  uniq -u ${filename}.1 > ${filename}.2
  #all with repetition
  uniq -d ${filename}.1 >> ${filename}.2

  #sort final list
  sort ${filename}.2 > ${filename}.3
}
#check if dependencies are ok
CheckNeeded sed || exit 1
CheckNeeded uniq || exit 1
CheckNeeded sort || exit 1

echo "build huge dictionary:"

#https://www.dictionary.com/list/a
fixLetterFile us_a.txt
fixLetterFile us_b.txt
fixLetterFile us_c.txt
fixLetterFile us_d.txt
fixLetterFile us_e.txt
fixLetterFile us_f.txt
fixLetterFile us_g.txt
fixLetterFile us_h.txt
fixLetterFile us_i.txt
fixLetterFile us_j.txt
fixLetterFile us_k.txt
fixLetterFile us_l.txt
fixLetterFile us_m.txt
fixLetterFile us_n.txt
fixLetterFile us_p.txt
fixLetterFile us_q.txt
fixLetterFile us_r.txt
fixLetterFile us_s.txt
fixLetterFile us_t.txt
fixLetterFile us_u.txt
fixLetterFile us_v.txt
fixLetterFile us_w.txt
fixLetterFile us_x.txt
fixLetterFile us_y.txt
fixLetterFile us_z.txt

cat us_a.txt.3 > us_dictionary.txt
cat us_b.txt.3 >> us_dictionary.txt
cat us_c.txt.3 >> us_dictionary.txt
cat us_d.txt.3 >> us_dictionary.txt
cat us_e.txt.3 >> us_dictionary.txt
cat us_f.txt.3 >> us_dictionary.txt
cat us_g.txt.3 >> us_dictionary.txt
cat us_h.txt.3 >> us_dictionary.txt
cat us_i.txt.3 >> us_dictionary.txt
cat us_j.txt.3 >> us_dictionary.txt
cat us_k.txt.3 >> us_dictionary.txt
cat us_l.txt.3 >> us_dictionary.txt
cat us_m.txt.3 >> us_dictionary.txt
cat us_n.txt.3 >> us_dictionary.txt
cat us_p.txt.3 >> us_dictionary.txt
cat us_q.txt.3 >> us_dictionary.txt
cat us_r.txt.3 >> us_dictionary.txt
cat us_s.txt.3 >> us_dictionary.txt
cat us_t.txt.3 >> us_dictionary.txt
cat us_u.txt.3 >> us_dictionary.txt
cat us_v.txt.3 >> us_dictionary.txt
cat us_w.txt.3 >> us_dictionary.txt
cat us_x.txt.3 >> us_dictionary.txt
cat us_y.txt.3 >> us_dictionary.txt
cat us_z.txt.3 >> us_dictionary.txt

count=$(wc -l ./us_dictionary.txt)
echo "total words in dictionary: ${count}"

rm *.1 *.2 *.3

#check diff
#meld us_a.txt us_a.txt.1
#meld us_a.txt.1 us_a.txt.3
