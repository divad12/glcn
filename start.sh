#!/bin/bash

#make && java -jar ../tools/PlayGame.jar ../maps/map7.txt 1000 1000 log.txt ./MyBot ../../1/MyBot | java -jar ../tools/ShowGame.jar
#make && java -jar ../tools/PlayGame.jar ../maps/map7.txt 1000 1000 log.txt ../../1/MyBot ./MyBot | java -jar ../tools/ShowGame.jar
#make && java -jar ../tools/PlayGame.jar ../maps/map7.txt 1000 1000 log.txt ./MyBot "java -jar ../example_bots/RandomBot.jar" | java -jar ../tools/ShowGame.jar
make -C mybot && java -jar tools/PlayGame.jar maps/map7.txt 1000 1000 log.txt "mybot/MyBot debug" "java -jar example_bots/DualBot.jar" | java -jar tools/ShowGame.jar
