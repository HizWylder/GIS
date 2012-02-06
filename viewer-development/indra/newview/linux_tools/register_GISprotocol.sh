#!/bin/bash

# Register a protocol handler (default: handle_GISprotocol.sh) for
# URLs of the form GIS://...
#

HANDLER="$1"

RUN_PATH=`dirname "$0" || echo .`
cd "${RUN_PATH}/.."

if [ -z "$HANDLER" ]; then
    HANDLER=`pwd`/etc/handle_GISprotocol.sh
fi

# Register handler for GNOME-aware apps
LLGCONFTOOL2=gconftool-2
if which ${LLGCONFTOOL2} >/dev/null; then
    (${LLGCONFTOOL2} -s -t string /desktop/gnome/url-handlers/GIS/command "${HANDLER} \"%s\"" && ${LLGCONFTOOL2} -s -t bool /desktop/gnome/url-handlers/GIS/enabled true) || echo Warning: Did not register GIS:// handler with GNOME: ${LLGCONFTOOL2} failed.
else
    echo Warning: Did not register GIS:// handler with GNOME: ${LLGCONFTOOL2} not found.
fi

# Register handler for KDE-aware apps
for LLKDECONFIG in kde-config kde4-config; do
    if [ `which $LLKDECONFIG` ]; then
        LLKDEPROTODIR=`$LLKDECONFIG --path services | cut -d ':' -f 1`
        if [ -d "$LLKDEPROTODIR" ]; then
            LLKDEPROTOFILE=${LLKDEPROTODIR}/GIS.protocol
            cat > ${LLKDEPROTOFILE} <<EOF || echo Warning: Did not register GIS:// handler with KDE: Could not write ${LLKDEPROTOFILE} 
[Protocol]
exec=${HANDLER} '%u'
protocol=GIS
input=none
output=none
helper=true
listing=
reading=false
writing=false
makedir=false
deleting=false
EOF
        else
            echo Warning: Did not register GIS:// handler with KDE: Directory $LLKDEPROTODIR does not exist.
        fi
    fi
done

