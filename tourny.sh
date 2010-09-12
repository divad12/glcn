#!/bin/bash

date >> results.txt

./playall.sh ../bots/7/MyBot ../bots/7-frugally-take-ops/MyBot ../bots/6/MyBot ../bots/5/MyBot ../bots/5-skip-first-turn/MyBot ../bots/5-unlimited-fleets/MyBot ../bots/4-no-fleets-limit/MyBot ../bots/3/MyBot example_bots/DualBot.jar example_bots/RageBot.jar >> results.txt

./playall.sh ../bots/7-frugally-take-ops/MyBot ../bots/7/MyBot ../bots/6/MyBot ../bots/5/MyBot ../bots/5-skip-first-turn/MyBot ../bots/5-unlimited-fleets/MyBot ../bots/4-no-fleets-limit/MyBot ../bots/3/MyBot example_bots/DualBot.jar example_bots/RageBot.jar >> results.txt

echo >> results.txt
