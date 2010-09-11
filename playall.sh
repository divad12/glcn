#!/bin/bash

mybot=$1
shift

echo "$mybot vs"

total_wins=0
total_draws=0
total_losses=0

for file in "$@"
do
    player_1_counter=0
    player_2_counter=0
    draws_counter=0
    player_2_turn_counter=0
    echo "Bot: $file" 1>&2
    for i in {1..100}
    do
        echo -n "M$i" 1>&2

        cmd=$file
        if [[ "$file" =~ .*\.jar$ ]]; then
            cmd="java -jar $file"
        fi

        RES=`java -jar tools/PlayGame.jar maps/map$i.txt 500 500 log.txt "$cmd" "$mybot" 2>&1 |
                 tail -n 3 | egrep "^Turn|^Player|^Draw"`
        TURN=`echo $RES | sed 's/Turn \([0-9]*\) .*/\1/g'`
        RES2=`echo $RES | egrep -o 'Player [0-9] Wins!|Draw!'`

        if [ "$RES2" = "Player 1 Wins!" ] ; then
            player_1_counter=`expr $player_1_counter + 1`
            echo -n '-LOST!' 1>&2
        elif [ "$RES2" = "Draw!" ] ; then
            echo -n '-DRAW!' 1>&2
            draws_counter=`expr $draws_counter + 1`
        elif [ "$RES2" = "Player 2 Wins!" ] ; then
            player_2_counter=`expr $player_2_counter + 1`
            player_2_turn_counter=`expr $player_2_turn_counter + $TURN`
        else
            echo -n "-$RES" 1>&2
        fi

        echo -n "-$TURN " 1>&2
    done

    echo 1>&2
    player_2_turn_counter=`expr $player_2_turn_counter / $player_2_counter`
    echo "    $file: $player_2_counter - $draws_counter - $player_1_counter, avg turns: $player_2_turn_counter"

    total_wins=`expr $total_wins + $player_2_counter`
    total_draws=`expr $total_draws + $draws_counter`
    total_losses=`expr $total_losses + $player_1_counter`
done

echo "    totals: $total_wins - $total_draws - $total_losses"

