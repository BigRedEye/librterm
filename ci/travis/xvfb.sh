#!/usr/bin/env bash

/sbin/start-stop-daemon --start --quiet --pidfile /tmp/custom_xvfb_99.pid --make-pidfile --background --exec /usr/bin/Xvfb -- :99 -ac -screen 0 1280x1024x16
export DISPLAY=:99.0
sh -e /etc/init.d/xvfb start
sleep 3
