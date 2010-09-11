#!/bin/bash

#make -C mybot && java -jar tools/PlayGame.jar maps/map7.txt 1000 1000 log.txt "mybot/MyBot debug" "java -jar example_bots/DualBot.jar" | java -jar tools/ShowGame.jar &
#make -C mybot && java -jar tools/PlayGame.jar maps/map7.txt 1000 1000 log.txt "mybot/MyBot debug" "../2/MyBot" | java -jar tools/ShowGame.jar &
#make -C mybot && java -jar tools/PlayGame.jar maps/map3.txt 1000 1000 log.txt "mybot/MyBot debug" "java -jar example_bots/DualBot.jar"
#make -C mybot && java -jar tools/PlayGame.jar maps/map39.txt 1000 1000 log.txt "java -jar example_bots/BullyBot.jar" "mybot/MyBot debug" | java -jar tools/ShowGame.jar
make -C mybot && java -jar tools/PlayGame.jar maps/map14.txt 1000 200 log.txt  "mybot/MyBot debug" "../bots/2/MyBot" | java -jar tools/ShowGame.jar
#make -C mybot && java -jar tools/PlayGame.jar maps/map39.txt 1000 100 log.txt  "java -jar example_bots/RageBot.jar" "java -jar example_bots/RageBot.jar" | java -jar tools/ShowGame.jar
