#! /bin/bash

#########################################################################
# Simple system check for email contact feedback or bug-report.         #
# Copyright (C) 2017  Brüggemann Eddie.                                 #
#                                                                       #
# This program is free software: you can redistribute it and/or modify  #
# it under the terms of the GNU General Public License as published by  #
# the Free Software Foundation, either version 3 of the License, or     #
# (at your option) any later version.                                   #
#                                                                       #
# This program is distributed in the hope that it will be useful,       #
# but WITHOUT ANY WARRANTY; without even the implied warranty of        #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
# GNU General Public License for more details.                          #
#                                                                       #
# You should have received a copy of the GNU General Public License     #
# along with this program.  If not, see <http://www.gnu.org/licenses/>. #
#########################################################################


#######################################################
#                                                     #
# Basic system check from the target user             #
# about the target system that the user can send me,  #
# if he want.                                         #
#                                                     #
# So I can know if the user mail me what's about.     #
#                                                     #
# It equal to not ask what he will **surely omit**.   #
# But what it can be important !                      #
#                                                     #
#######################################################
#                                                     #
# @Important: Do not worry I'm not a silly criminal   #
# or like. I want just  automate additionnal          #
# informations asking about your system, maybe needed #
# if you mail me.                                     # 
#                                                     #
#######################################################
#                                                     #
# You can add some informations from your own if you  #
# want. Per example the version number from glib-2.0  #
# and from gio-2.0 would pertains in this case.       #
#                                                     #
#######################################################

# @NOTE: whoami, users, id # for getting the [user|host]name would work too.

# Output filename.
filename=whoami.info

# Check environment variable to getthe user name:
if [[ -n ${LOGNAME} ]] ; then

  echo -n "log name: " > ${filename}

  echo "${LOGNAME}" >> ${filename}

  echo "" >> "${filename}"
  echo "---" >> "${filename}"
  echo "" >> "${filename}"

elif [[ -n ${USER} ]] ; then

  echo -n "user name: " > ${filename}

  echo "${USER}" >> ${filename}

  echo "" >> "${filename}"
  echo "---" >> "${filename}"
  echo "" >> "${filename}"

else

  var=`which whoami 2> /dev/null`

  if [[ -n ${var} ]] ; then

    echo -n "user name: " > ${filename}

    echo whoami >> ${filename}

    echo "" >> "${filename}"
    echo "---" >> "${filename}"
    echo "" >> "${filename}"

  fi

fi

# Checking the location from user.
var=`which locale 2> /dev/null`

if [[ -n ${var} ]] ; then

  echo "Localisation: " >> ${filename}
  echo "" >> ${filename}

  var1=`which awk`
  var2=`which gawk`
  if [[ -n ${var1} ]] ; then

    echo locale | "$(awk '{print $1}' )"  >> ${filename}

    echo "" >> "${filename}"
    echo "---" >> "${filename}"
    echo "" >> "${filename}"

  elif [[ -n ${var2} ]] ; then

    echo locale | "$(gawk '{print $1}' )"  >> ${filename}

    echo "" >> "${filename}"
    echo "---" >> "${filename}"
    echo "" >> "${filename}"

  else

    echo locale >> ${filename}

    echo "" >> "${filename}"
    echo "---" >> "${filename}"
    echo "" >> "${filename}"

  fi


fi


# Getting more information on user.
var=`which pinky 2> /dev/null`

if [[ -n ${var} ]] && [[ -n ${LOGNAME} ]] ; then

  echo "finger like :" >> ${filename}
  echo "" >> ${filename}

  pinky -l $LOGNAME >> "${filename}"

  echo "" >> "${filename}"
  echo "---" >> "${filename}"
  echo "" >> "${filename}"

elif [[ -n ${var} ]] && [[ -n ${USER} ]] ; then

  echo "finger like :" >> ${filename}
  echo "" >> ${filename}

  pinky -l ${USER} >> "${filename}"

  echo "" >> "${filename}"
  echo "---" >> "${filename}"
  echo "" >> "${filename}"

fi

# Define a simple function for the single statement binaries.
function simple_get_user_infos() {

  # Test if the binary exists on the target system:
  var=`which $1 2> /dev/null`

  if [[ -n ${var} ]] ; then
  
    # Binary present on the target system.
  
    # Writing a string.
    echo $2  >> ${filename}
    echo ""  >> ${filename}
  
    # if $3 (option) not defined ignore it.
    $1 $3 >> ${filename}
  
    # Separator.
    echo "" >> "${filename}"
    echo "---" >> "${filename}"
    echo "" >> "${filename}"

  fi

}

simple_get_user_infos who "Connected user(s) :" -a

simple_get_user_infos date "date : " -R

simple_get_user_infos uname "uname : " -a

simple_get_user_infos arch "architecture : "

simple_get_user_infos nproc "number of processors : "

# I know no basic (built in: coreutils) to get this kind of information.
while [[ -z ${distribution} ]]

do

  # No basic command for detection without add-on's !
  echo "On what system or GNU/Linux distribution and version you be (Be precise please)."

  echo -n ">>> "

  read distribution

done

echo -n "distribution: " >> "${filename}"

echo "${distribution}" >> "${filename}"

echo "" >> "${filename}"

echo ""

echo "Thanks to set the content of the file ${filename} as mail footer"
echo "or as attachment and thanks for mailing me."

exit 0

