#!/bin/bash

for file in example_bots/*.jar
do
    player_1_counter=0
    player_2_counter=0
    echo "Bot: $file"
    for i in {1..100}
    do
        echo "Map: $i"
        RES=`java -jar tools/PlayGame.jar maps/map$i.txt 1000 1000 log.txt "java -jar $file" "mybot/MyBot" 2>&1 | grep ^Player`
        if [ "$RES" = "Player 1 Wins!" ] ; then
            player_1_counter=`expr $player_1_counter + 1`
        else
            player_2_counter=`expr $player_2_counter + 1`
        fi

        echo $RES;
    done
    echo "$file : $player_2_counter/100"
done
