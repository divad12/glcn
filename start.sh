#!/bin/bash

rageBot="java -jar example_bots/RageBot.jar"
prospectorBot="java -jar example_bots/ProspectorBot.jar"
myBot="mybot/MyBot"


#make -C mybot && java -jar tools/PlayGame.jar maps/map7.txt 1000 1000 log.txt "mybot/MyBot debug" "java -jar example_bots/DualBot.jar" | java -jar tools/ShowGame.jar &
#make -C mybot && java -jar tools/PlayGame.jar maps/map7.txt 1000 1000 log.txt "mybot/MyBot debug" "../2/MyBot" | java -jar tools/ShowGame.jar &
#make -C mybot && java -jar tools/PlayGame.jar maps/map3.txt 1000 1000 log.txt "mybot/MyBot debug" "java -jar example_bots/DualBot.jar"
#make -C mybot && java -jar tools/PlayGame.jar maps/map39.txt 1000 1000 log.txt "java -jar example_bots/BullyBot.jar" "mybot/MyBot debug" | java -jar tools/ShowGame.jar
#make -C mybot && java -jar tools/PlayGame.jar maps/map22.txt 1000 200 log.txt "../bots/4/MyBot" "$rageBot" | java -jar tools/ShowGame.jar
#java -jar tools/PlayGame.jar maps/mapGoodCentre.txt 1000 20 log.txt  "$prospectorBot" "$prospectorBot" | java -jar tools/ShowGame.jar
make -C mybot && java -jar tools/PlayGame.jar maps/map6.txt 1000 100 log.txt  "mybot/MyBot debug" "../bots/4/MyBot" | java -jar tools/ShowGame.jar



# For unit testing
#make -C mybot && java -jar tools/PlayGame.jar maps/map14.txt 1000 1 log.txt  "mybot/MyBot debug" "../bots/4/MyBot" && echo "------- debug file below: ------" && cat debug.txt
