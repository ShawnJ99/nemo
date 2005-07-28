# setup manybody environment for sh

# you really should hardcode your address in here
export MANYBODY=`pwd`

export NEMO=$MANYBODY/nemo_cvs
export STARLAB=$MANYBODY/starlab_cvs
export ACSROOT=$MANYBODY/acs

# check for existence
if [ -e $NEMO/nemo_start.sh ]
then
  source $NEMO/nemo_start.sh
else
  echo Warning: no NEMO present
fi

if [ -e $STARLAB/starlab_start.sh ]
then
  source $STARLAB/starlab_start.sh
else
  echo Warning: no STARLAB present
fi

PATH=$MANYBODY/opt/bin:$PATH
LD_LIBRARY_PATH=$MANYBODY/opt/lib:$LD_LIBRARY_PATH
