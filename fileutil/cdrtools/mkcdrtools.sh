#!/bin/sh
#==============================================================================
# mikebren@caldera.com				last change:	     2001/12/12
#==============================================================================
#
# This is a shell script to create a Skunkware distribution of cdrtools.
#
#==============================================================================

PROGNAME=`basename $0`

#===================================

usage () {

cat | more << EOF
#
# $PROGNAME: shell script to create a Skunkware distribution of cdrtools
#
# In order to run this you should have the following software installed:
#
# a) Hosting OS with recommended patches 
#
#       - OpenServer 5.0.5 or newer
#       - UnixWare 7.1.1 or newer   (UnixWare 2.1.X will not compile !)
#       - OpenUnix 8.0 or newer
#
# b) Native Development System 
#
#	- OpenServer DevSys for Openserver
#	- UODK for UnixWare & OpenUnix
#
# c) Skunkware Tools
#
#	- perl (necessary for man2html)
#	- man2html
#	- smake (best for compiling any software from Joerg Schilling)
#	- gzip (for compression we could also use compress, but as the
#		original source comes as gzip'ed tar archive anyway,
#		lets use gzip also for the skunkware distribution pkgs)
#
#
# To built the binary and src distribution archives, you should follow 
# these steps (do it as user ROOT !!! otherwise the owner/group are wrong):
#
# a) extract a cdrtools-??? (for this example let's assume cdrtools-1.11a12)
#    source archive to any directory, let's assume /home/cdrecord, so the 
#    extracted source would have created a /home/cdrecord/cdrtools-1.11 
#    directory. 
#    
#    As this would be the case for all cdrtools-1.11a?? versions, move
#    the directory to the real archive name to have the ability to hold
#    several cdrtools-1.11a?? versions concurrently. 
#
#    So move the directory in this example like:
#
#    #mv /home/cdrecord/cdrtools-1.11 /home/cdrecord/cdrtools-1.11a12
#
# b) Then cd into this directory and run this script!
#
# c) If the scripts ends, you'll find two archives in the directory 
#
#       /tmp/cdrtools-1.11a12 
#
#    as well as a build-log:
#
#    cdrtools-1.11a12-bld.log		- the build logfile
#    cdrtools-1.11a12-bin.tar.gz	- the binary distribution archive
#    cdrtools-1.11a12-src.tar.gz	- the source distribution archive

EOF
	
}

#===================================

build_environment () {

	CURRDIR=`pwd`
	VERSION=`basename $CURRDIR`

	echo "--------------------------------------------"
	echo "building -> " $VERSION " <- for Skunkware   "
	echo "--------------------------------------------"
	
	BASEDIR=/tmp/$VERSION
	
	if [ -f $BASEDIR ]
	then
		echo
		echo "There is a file called $BASEDIR, can't create directory!"
		echo
		exit
	fi

	#
	# if the directory already exists, we assume it's from 
	# a previous run, so we just delete it. 
	#

	if [ -d $BASEDIR ]
	then
		rm -rf $BASEDIR
	fi

	# ----------------------------------
	# set platform dependend binary path
	#

	case `uname` in
	
	  UnixWare)     OBJ="OBJ/ia32-unixware-cc"
	  		OPS="uw7"
	                ;;
                
	  OpenUNIX)     OBJ="OBJ/ia32-unixware-cc"
	  		OPS="ou8"
	                ;;
                
	  SCO_SV)       OBJ="OBJ/i386-openserver-cc"
	  		OPS="os5"
	                ;;
	esac

	BIN_ARC=$BASEDIR/$VERSION-$OPS-bin.tar
	SRC_ARC=$BASEDIR/$VERSION-$OPS-src.tar
	BLD_LOG=$BASEDIR/$VERSION-$OPS-bld.log
	
	USR=usr
	ETC=etc

	BIN=$BASEDIR/$USR/local/bin
	MAN=$BASEDIR/$USR/local/man
	DEF=$BASEDIR/$ETC/default

	MAN1=$MAN/man.1
	MAN8=$MAN/man.8
	CAT1=$MAN/cat.1
	CAT8=$MAN/cat.8
	HTML=$MAN/html

	mkdir -p $BASEDIR

	mkdir -p $BIN
	mkdir -p $DEF
	mkdir -p $MAN1
	mkdir -p $MAN8
	mkdir -p $CAT1
	mkdir -p $CAT8
	mkdir -p $HTML

	echo "CURRDIR = " $CURRDIR
	echo "VERSION = " $VERSION
	echo "BASEDIR = " $BASEDIR
	echo "BIN_ARC = " $BIN_ARC
	echo "SRC_ARC = " $SRC_ARC
	echo "BLD_LOG = " $BLD_LOG
	echo "USR     = " $USR
	echo "ETC     = " $ETC
	echo "BIN     = " $BIN
	echo "MAN     = " $MAN
	echo "DEF     = " $DEF
	echo "OBJ     = " $OBJ
	echo "OPS     = " $OPS
	echo "MAN1    = " $MAN1
	echo "MAN8    = " $MAN8
	echo "CAT1    = " $CAT1
	echo "CAT8    = " $CAT8
	echo "HTML    = " $HTML
	
}

#===================================

build_compilation () {

	echo "--------------------------------------------"
	echo "compiling -> " $VERSION 
	echo "--------------------------------------------"

	# make the sourcetree distclean !
	# -------------------------------
	echo "run .clean ..."; ./.clean

	# run the schilly make and log it
	# -------------------------------
	echo "run smake ..."; smake 2>&1 | tee $BLD_LOG

}

#===================================

build_distribution () {
	
	echo "--------------------------------------------"
	echo "building -> " $VERSION " distribution"
	echo "--------------------------------------------"

	cp cdrecord/$OBJ/cdrecord	$BIN
	cp readcd/$OBJ/readcd		$BIN
	cp mkisofs/$OBJ/mkisofs		$BIN
	cp mkisofs/diag/$OBJ/isodump	$BIN
	cp mkisofs/diag/$OBJ/isoinfo	$BIN
	cp mkisofs/diag/$OBJ/isovfy	$BIN
	cp mkisofs/diag/$OBJ/devdump	$BIN
	cp scgcheck/$OBJ/scgcheck	$BIN

	cp cdrecord/cdrecord.1		$MAN1
	cp readcd/readcd.1		$MAN1
	cp scgcheck/scgcheck.1		$MAN1

	cp mkisofs/mkisofs.8		$MAN8
	cp mkisofs/diag/isoinfo.8	$MAN8

	MANPATH=$MAN man cdrecord > /dev/null 2> /dev/null
	MANPATH=$MAN man mkisofs  > /dev/null 2> /dev/null
	MANPATH=$MAN man isoinfo  > /dev/null 2> /dev/null
	MANPATH=$MAN man readcd   > /dev/null 2> /dev/null
	MANPATH=$MAN man scgcheck > /dev/null 2> /dev/null

	man2html < $CAT1/cdrecord.1 > $HTML/cdrecord.html
	man2html < $CAT8/mkisofs.8  > $HTML/mkisofs.html
	man2html < $CAT1/readcd.1   > $HTML/readcd.html
	man2html < $CAT8/isoinfo.8  > $HTML/isoinfo.html
	man2html < $CAT1/scgcheck.1 > $HTML/scgcheck.html

	cat > $DEF/cdrecord <<EOF
# --------------------------------------------------------------------------
# Sample /etc/default/cdrecord file, please adjust to your setup and don't
# forget to remove the commenting '#' chars in front of the definitions !!
# --------------------------------------------------------------------------


# --------------------------------------------------------------------------
# default device / default speed / default fifo size
# --------------------------------------------------------------------------

#CDR_DEVICE=1,0,0
#CDR_SPEED=16
#CDR_FIFOSIZE=8m

# --------------------------------------------------------------------------
# Sample Identifier for a TEAC CDR56S with speed 6 and a fifo size of 4 MB
# --------------------------------------------------------------------------

#Teac=0,5,0      6       4m

# --------------------------------------------------------------------------
# Sample Identifier for Yamaha CDRW4416S with speed 4 and a fifo size of 8MB
# --------------------------------------------------------------------------

#Yamaha=2,6,0      4       8m

EOF

}

#===================================

create_bin_archive () {

	cd $BASEDIR
	echo "creating binary archive ..."
	tar cf $BIN_ARC ./$USR ./$ETC
	gzip -9 -f $BIN_ARC
	rm -rf ./$USR
	rm -rf ./$ETC
	cd $CURRDIR
}

#===================================

create_src_archive () {

	cd $CURRDIR
	./.clean
	cd ..
	echo "creating source archive ..."
	tar cf $SRC_ARC ./$VERSION
	gzip -9 -f $SRC_ARC
	cd $CURRDIR
}


#===================================
# main
#===================================

if [ $# -gt 0 ]
then
	usage
	exit
fi

if [ ! -f ./cdrecord/cdrecord.c ]
then
	echo 
	echo "Can't find ./cdrecord/cdrecord.c, so we are not in "
	echo "a cdrtools source directory !! Check/Adjust/Restart !!"
	echo
	exit
fi

build_environment  
build_compilation  
build_distribution

#
# if /home/bin exists, copy the binaries there for testing
#

if [ -d /home/bin ] 
then
	cp $BIN/* /home/bin
fi

create_bin_archive 
create_src_archive 
echo 
ls $BASEDIR
echo
echo "done !"

#===================================
# end of script
#===================================
