#!/bin/bash

# Send a URL of the form GIS://... to GIS.
#

URL="$1"

if [ -z "$URL" ]; then
    echo Usage: $0 GIS://...
    exit
fi

RUN_PATH=`dirname "$0" || echo .`
cd "${RUN_PATH}/.."

exec ./GIS -url \'"${URL}"\'

