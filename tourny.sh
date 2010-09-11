#!/bin/bash

date >> results.txt

./playall.sh mybot/MyBot ../bots/2/MyBot ../bots/3/MyBot example_bots/BullyBot.jar example_bots/DualBot.jar example_bots/ProspectorBot.jar example_bots/RageBot.jar >> results.txt
./playall.sh ../bots/3/MyBot ../bots/2/MyBot example_bots/BullyBot.jar example_bots/DualBot.jar example_bots/ProspectorBot.jar example_bots/RageBot.jar >> results.txt

echo >> results.txt
