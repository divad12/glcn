#!/bin/bash

date >> results.txt

./playall.sh ../bots/5/MyBot ../bots/5-issueOrder-simple/MyBot ../bots/4/MyBot ../bots/4-no-fleets-limit/MyBot ../bots/3/MyBot example_bots/BullyBot.jar example_bots/DualBot.jar example_bots/ProspectorBot.jar example_bots/RageBot.jar >> results.txt

echo >> results.txt
