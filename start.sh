#!/bin/bash

#make -C mybot && java -jar tools/PlayGame.jar maps/map7.txt 1000 1000 log.txt "mybot/MyBot debug" "java -jar example_bots/DualBot.jar" | java -jar tools/ShowGame.jar
#make -C mybot && java -jar tools/PlayGame.jar maps/map7.txt 1000 1000 log.txt "mybot/MyBot debug" "../2/MyBot" | java -jar tools/ShowGame.jar


make -C mybot && java -jar tools/PlayGame2.jar maps/map3.txt 1000 1000 log.txt "mybot/MyBot debug" "java -jar example_bots/DualBot.jar"
