#!/bin/bash
#set -x

PROC="mq_server"
SUFFIX=".bin"
MCP=$PROC"_mcp"$SUFFIX
SCC=$PROC"_scc"$SUFFIX
CCS=$PROC"_ccs"$SUFFIX

doStart()
{
	echo start
	ALLRPOC="$MCP $SCC $CCS"
	for BIN in $ALLRPOC
	do
		echo $BIN
		BIN_NAME=`basename -s $SUFFIX $BIN`
		SCRIPT_NAME=`basename $0`
		NUM=`ps -fu ${USER} |grep $BIN" ../etc/"$BIN_NAME".cfg" |grep -v grep|grep -v $SCRIPT_NAME|wc -l`
		if [ $NUM -le 0 ]
		then
			echo "./$BIN ../etc/$BIN_NAME.cfg"
			./$BIN ../etc/$BIN_NAME.cfg
		fi
	done
}


doStop()
{
	echo "stoping ${PROC}...!"

	PIDS=`pidof $MCP $CCS $SCC`
	echo $PIDS

	KILL=0
	for PID in $PIDS
	do
		kill -s USR2 $PID
		KILL=1
	done

	if [ $KILL = 1 ]
	then
		echo "stop ${PROC} ok!"
	else
		echo "Nothing stop!"
	fi

	########## clear shm #######
	#./clearshm.sh 0
}

doStatus()
{

	ps -ef | grep -E "$MCP|$CCS|$SCC" | grep -v grep
}

opt=$1
case "$opt" in
	"start" )
		doStart
		;;
	"stop" )
		echo stop
		doStop
		;;
	"status" )
		doStatus
		;;
	"restart" )
		doStop
		doStart
	;;
	#TODO: other options
esac

