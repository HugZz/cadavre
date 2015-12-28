# cadavre #

## Exquisite corpse game using C socket.

From [Wikipedia](https://en.wikipedia.org/wiki/Exquisite_corpse):
> Exquisite corpse, also known as exquisite cadaver (from the original French term cadavre exquis) or rotating corpse,
> is a method by which a collection of words or images is collectively assembled. 
> Each collaborator adds to a composition in sequence, either by following a rule 
> (e.g. "The adjective noun adverb verb the adjective noun", as in "The green duck sweetly sang the dreadful dirge") 
> or by being allowed to see only the end of what the previous person contributed.

## How to play ##

Download the game with git:
* `sudo apt-get install git`
* `git clone https://github.com/HugZz/cadavre.git`
* `cd cadavre`

To play one game, you first need to launch the server on a machine connected to a network:
* `cd server`
* `make`
* `./cadavre-server PORT_NUMBER`
Make sure to choose a port number that is not already used by any application running.
Note the hostname or the IP address of the server you used to let the players (the clients) connect and play.

To connect clients (players):
* `cd client`
* `make`
* `./cadavre HOSTNAME PORT_NUMBER`
Use the hostname of your server and the same port number.

## Contributions ##

Feel free to clone this repository and improve it, I will be happy to receive any pull request.
