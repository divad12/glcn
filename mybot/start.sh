#!/bin/bash

#make && java -jar ../tools/PlayGame.jar ../maps/map7.txt 1000 1000 log.txt ./MyBot ../../1/MyBot | java -jar ../tools/ShowGame.jar
make && java -jar ../tools/PlayGame.jar ../maps/map7.txt 1000 1000 log.txt ../../1/MyBot ./MyBot | java -jar ../tools/ShowGame.jar
