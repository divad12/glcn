#!/bin/bash

mybot=$1
shift

for file in "$@"
do
    player_1_counter=0
    player_2_counter=0
    player_2_turn_counter=0
    echo "Bot: $file" 1>&2
    for i in {1..100}
    do
        echo -n "M$i" 1>&2

        cmd=$file
        if [[ "$file" =~ .*\.jar$ ]]; then
            cmd="java -jar $file"
        fi

        RES=`java -jar tools/PlayGame2.jar maps/map$i.txt 500 500 log.txt "$cmd" "$mybot" 2>&1 |
                 tail -n 3 | egrep "^Turn|^Player"`

        TURN=`echo $RES | sed 's/Turn \([0-9]*\) .*/\1/g'`
        RES2=`echo $RES | grep 'Player [0-9] Wins!'`
        if [ "$RES2" = "Player 1 Wins!" ] ; then
            player_1_counter=`expr $player_1_counter + 1`
            echo -n '-LOST!' 1>&2
        else
            player_2_counter=`expr $player_2_counter + 1`
            player_2_turn_counter=`expr $player_2_turn_counter + $TURN`
        fi
        echo -n "-$TURN " 1>&2

    done
    player_2_turn_counter=`expr $player_2_turn_counter / 100`
    echo "$file : $player_2_counter/100, avg turns: $player_2_turn_counter"
done

