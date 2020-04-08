#!/bin/sh

if ls ~/.iAloy_update > /dev/null; then
	chmod 777 ~/.iAloy_update/install
	~/.iAloy_update/install
	rm -rf ~/.iAloy_update
fi