#!/bin/bash
set -e
# INFO: We recommend running this script via the invoke task 'make-installers'
# to ensure that all necessary files and configurations are created.

VERSION=$1
CONFIG=${2:-Release}

BUILD_DIR="build/${CONFIG}"
INSTALLER_DIR="Installers"
STAGE_DIR="STAGE"
STAGE_LOCATION="${STAGE_DIR}/Release"

RELEASE_LOCATION="${INSTALLER_DIR}/Release"


rm -rfv $RELEASE_LOCATION
mkdir -pv $RELEASE_LOCATION
mkdir -pv $STAGE_LOCATION

cp -r $BUILD_DIR/bin/Resources $STAGE_LOCATION
cp -P $BUILD_DIR/lib/libDL180* $STAGE_LOCATION
cp -P $BUILD_DIR/lib/libicu* $STAGE_LOCATION
cp $BUILD_DIR/bin/pdfrotate $STAGE_LOCATION

cd $STAGE_DIR
tar cvf CLU_PDF_Rotate_"${VERSION}".tar -C Release .

cd -
mv "${STAGE_DIR}/CLU_PDF_Rotate_"${VERSION}".tar" "${RELEASE_LOCATION}/CLU_PDF_Rotate_"${VERSION}".tar"

rm -rf $STAGE_DIR
