#!/bin/bash
# create multiresolution windows icon
ICON_DST=../../src/qt/res/icons/daytradercoin.ico

convert ../../src/qt/res/icons/daytradercoin-16.png ../../src/qt/res/icons/daytradercoin-32.png ../../src/qt/res/icons/daytradercoin-48.png ${ICON_DST}
