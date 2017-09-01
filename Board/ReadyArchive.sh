#!/bin/bash
PlotFolder="Plot"
Boardname="Xmotion"
Folder=$1 #Output Folder
Pwd=`pwd`
mkdir $Folder/ 2> /dev/null
function missing()
{
	echo -e " $Boardname.$1 --> Layer Missing!"
	return 0
}

if [ ! -f $PlotFolder/$Boardname-B.Cu.gbr ]
	then
		missing gbl
	else
		cp $PlotFolder/$Boardname-B.Cu.gbr $Folder/$Boardname.GBL
fi

if [ ! -f $PlotFolder/$Boardname-F.Cu.gbr ]
	then
		missing gtl
	else
		cp $PlotFolder/$Boardname-F.Cu.gbr $Folder/$Boardname.GTL
fi

if [ ! -f $PlotFolder/$Boardname-F.Mask.gbr ]
	then
		missing gts
	else
		cp $PlotFolder/$Boardname-F.Mask.gbr $Folder/$Boardname.GTS
fi

if [ ! -f $PlotFolder/$Boardname-B.Mask.gbr ]
	then
		missing gbs
	else
		cp $PlotFolder/$Boardname-B.Mask.gbr $Folder/$Boardname.GBS
fi

if [ ! -f $PlotFolder/$Boardname-F.SilkS.gbr ]
	then
		missing gto
	else
		cp $PlotFolder/$Boardname-F.SilkS.gbr $Folder/$Boardname.GTO
fi

if [ ! -f $PlotFolder/$Boardname-Edge.Cuts.gbr ]
	then
		missing gko
	else
		cp $PlotFolder/$Boardname-Edge.Cuts.gbr $Folder/$Boardname.GKO
fi
#Uncomment for Inner Layers
#if [ ! -f $PlotFolder/$Boardname-Inner1.Cu.g3 ]
#	then
#		missing g2l
#	else
#		cp $PlotFolder/$Boardname-Inner1.Cu.g3 $Folder/$Boardname.G2L
#fi

#if [ ! -f $PlotFolder/$Boardname-Inner2.Cu.g2 ]
#	then
#		missing g3l
#	else
#		cp $PlotFolder/$Boardname-Inner2.Cu.g2 $Folder/$Boardname.G3L
#fi

cat $PlotFolder/$Boardname.drl > $Folder/$Boardname.XLN
#cat $PlotFolder/$Boardname-NPTH.drl >> $Folder/$Boardname.XLN

#cat $PlotFolder/$Boardname-top.pos > $Folder/$Boardname-Top.pos
#cat $PlotFolder/$Boardname-bottom.pos > $Folder/$Boardname-Bottom.pos
#rm Centroid.zip &> /dev/null
#cd $PlotFolder
#zip ../Centroid.zip *.pos
#cd ..

cd $Folder/
rm ../$Boardname.zip &> /dev/null
zip ../$Boardname.zip *.*
cd $Pwd
date >> ReadyArchive.log
md5sum $Boardname.zip >> ReadyArchive.log
